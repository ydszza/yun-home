#include "lcd.h"

/*
	lcd_init();
	功能：实现屏幕的初始化 即打开和映射屏幕 
	参数：无
	返回值：成功返回0 失败返回 -1  
*/
int lcd_init(void)
{
    //打开屏幕 
    lcd_fd = open("/dev/ubuntu_lcd",O_RDWR);
    if(lcd_fd < 0)
    {
        perror("open lcd error");
        return -1;
    }
    
    //映射屏幕 
    plcd = (int*)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);
    if(plcd == NULL)
    {
        perror("mmap lcd error");
        close(lcd_fd);
        return -1;
    }
    return 0;
}

/*	
	lcd_uninit()
	功能：lcd 屏幕的解除初始化  即解除映射 以及关闭屏幕 
	参数：无
	返回值：无 
*/	
int lcd_uninit(void)
{
    //解除映射 
    int ret = munmap(plcd,800*480*4);
    if(ret < 0)
    {
        perror("munmap error");
        return -1;
    }

    //关闭屏幕文件
    ret = close(lcd_fd);
    if(ret < 0)
    {
        perror("close lcd error");
        return -1;
    }
    return 0;
}

/*
	lcd_draw_point()
	函数功能：在屏幕上实现显示一个像素点
	函数参数：
			这个点的坐标 即 x y x表示行坐标  y表示列坐标 
			这个点显示什么颜色 color 
	函数返回值：无 
*/	
void lcd_draw_point(int x,int y,int color)
{
    int offset = x*800+y;
    if(offset >=480*800){
        printf("x=%d y=%d超出屏幕显示范围...............\n",x,y);
        return;
    }
    *(plcd+offset) = color; //画点
}

/*	
	lcd_draw_block()
	函数功能：在屏幕上画一块内容 与清屏有点类似
	参数： 
		画这一块内容的起始坐标 一定是左上角的坐标 即x0 y0 x0 表示行坐标 y0表示列坐标
		画的这个块的大小 即 宽 和 高 宽 使用 width  高使用 high 表示 
		块的颜色  color 
	返回值： 无 
*/	
void lcd_draw_block(int x0,int y0,int width,int high,int color)
{
    //如何画这一块 首先起点坐标确定 即  i j的初始值知道 
    int i,j;
    for(i=x0;i< x0+high ;i++)
    {
        for(j=y0;j< y0+width;j++)
        {
            lcd_draw_point(i,j,color);
        }
    }
}


/*
    功能：显示一个文字
    参数：x0 y0 为起始的地址 
            width 即为宽度 表示 这个字体显示一行中有多个个像素点 
            size 即表示这个字 的数组其大小为多少字节
            word_name 即自己的 名字
*/
void draw_word(int x0,int y0,int width,int size,char word_name[],int color)
{	
    int flag = width / 8; //表示一行占用几个字节 
    
    int i,j;
    for(i=0;i<size;i++) //遍历每一个字节 
    {
        for(j=0;j<8;j++) //遍历每一位
        {
            if(word_name[i]>>j & 0x01) //成立表示为 1  则画点
            {
                int x,y;
                x = x0+i/flag;
                y = y0+i%flag*8+(7-j);
                if(x*800+y > 480*800-1){
                    printf("        要显示的内容超出屏幕范围！！！！！！！！！！！\n");
                    return;
                }
                lcd_draw_point(x, y , color); //确定坐标和颜色 
            }
            else //不成立 表示为0 则什么也不做  
            {
            }
        }
    
    }
}

/*
    功能：按行显示多个文字
    参数：x0 y0 为起始的地址 
        width 即为宽度表示这个字体显示一行中有多少个像素点 
        size 即表示一个字的大小为多少字节
        cnt 自己的名字有几个字
        words 即自己的名字
    返回值：无
*/
void draw_words_row(int x0, int y0, int width, int size, int cnt, char words[], int color)
{
    int i;
    for(i=0; i<cnt; i++){
        draw_word(x0, y0+i*width, width, size, words+i*size, color);
    }

}

/*
    功能：按列显示多个文字
    参数：x0 y0 为起始的地址 
        width 即为宽度表示这个字体显示一行中有多少个像素点 
        size 即表示一个字的大小为多少字节
        cnt 自己的名字有几个字
        words 即自己的名字
    返回值：无
*/
void draw_words_column(int x0, int y0, int width, int size, int cnt, char words[], int color)
{
    int i;
    for(i=0; i<cnt; i++){
        draw_word(x0+i*width, y0, width, size, words+i*size, color);
    }

}

/*
 * show_bmp()
 * 函数功能：在屏幕指定显示一张图片
 * 参数：
    1、即图片的名字 char *pathname 即带路径的文件名进行传参即可 
	2、 图片显示的起始位置 即 第x0行 y0列 
	3、图片的宽width和高height 注意 图片的宽必须是4的整数倍
 * 返回值：0成功， -1失败
 */
int show_bmp(char *pathname, int x0, int y0, int width, int height)
{
    //打开图片文件
    int pic_fd = open(pathname, O_RDWR);
    if (pic_fd < 0) {
        perror("open pic error");
        return -1;
    }

    //移动光标跳过图片信息
    int ret = lseek(pic_fd, 54, SEEK_SET);
    if(ret < 0){
        perror("lseek");
        close(pic_fd);
        return -1;
    }

    //读取图片内容
    char pic_buf[800*480*3] = {0};
    ret = read(pic_fd, pic_buf, 800*480*4);
    if(ret < 0){
        perror("read");
        close(pic_fd);
        return -1;
    }

    //在屏幕上显示图片
    unsigned int a,r,g,b,color;
	int i,j; 
	unsigned char *p = pic_buf; 
	for(i = 0;i<height;i++)
	{
		for(j =0;j<width;j++)
		{
			a = 0x00;
			b = *p++; 
			g = *p++;
			r = *p++;
			color = a<<24 | r<<16 | g<<8 | b;
			lcd_draw_point(x0+height-1-i,y0+j,color); 
		}
	}

	//关闭文件 
	close(pic_fd);
    return 0;
}

/*
 *get_touch()
 * 函数功能：获取在屏幕上的点击位置
 * 参数：坐标位置结构体指针
 * 返回值：0成功， -1失败
 */
int get_touch(struct point *position)
{
    //清楚上次坐标信息
    position->x = -1;
    position->y = -1;

    //触摸设备文件描述符
    int ts_fd = -1;
    //打开触摸设备文件
    ts_fd = open("/dev/ubuntu_event", O_RDONLY);
    if(ts_fd < 0){
        perror("open touch");
        return -1;
    }    

    //获取屏幕点击信息结构体
    struct input_event data;
    memset(&data, 0, sizeof(data));

    //printf("kaishi   %d\n", ts_fd);

    int ret;
    //循环获取点击位置信息
    while(1) {
        //读取点击信息
        ret = read(ts_fd, &data, sizeof(data));
        if(ret < sizeof(data)){
            printf("read failed\n");
            continue;
        }

        //解析点击信息，获取点击的坐标
        //此处获取的x y值x为列值，y为行值
        if(data.type == EV_ABS){//表示一个绝对事件，即屏幕点击事件
            if(data.code == ABS_X){//此时信息是x轴坐标
                if((position->x < 0) && (position->y < 0)){
                    position->y = data.value;
                }
            }else if(data.code == ABS_Y){//此时信息是y轴坐标
                if((position->x < 0) && (position->y >= 0)){
                    position->x = data.value;
                    break;
                }
            }
        }
    }

    //printf("x= %d, y= %d\n",position->x,position->y);

    //关闭屏幕触摸文件
    ret = close(ts_fd);
    if(ret < 0){
        perror("close touch failed");
        return -1;
    }
    return 0;
}
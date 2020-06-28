#include "show_smoke.h"

/*
 * 显示烟雾浓度界面子程序
 * 显示烟雾浓度提示语
 * 展示烟雾浓度数据
 * 烟雾浓度过大提醒
 **/
void show_smoke_page(void)
{
    //进入本子程序清屏为白色 
	lcd_draw_block(0,0,800,480,0x00FFFFFF);
    //draw_words_row(150, 350, 32, 32*29/8, 4, smoke_text, 0x00000000);
    //显示烟雾浓度背景及文字
    // show_bmp("./pic/smoke.bmp", 0, 0,800,480);
    show_bmp("./bmp/smoke.bmp", (480-416)/2, (800-744)/2, 744, 416);
    //lcd_draw_block(360, 120, 50, 40, 0x000000ff);//退出
    // //显示返回图标
    // show_bmp("./pic/back.bmp", 400,500,40,40);

    //存储烟雾浓度变量
    int smoke_data;
    //每一秒获取一次烟雾浓度数据并显示
    while(1){
        //获取数据
        smoke_data = get_smoke_data();
        
        //连接云端成功发送数据，否则跳过
        if(sock_fd != -1){
            struct sock_data_t send_data;
            memset(&send_data, 0, sizeof(send_data));
            send_data.flag = 0;//标志位为0表示为smoke数据
            send_data.data[0] = smoke_data;
            //发送数据至服务器
            int ret = send(sock_fd, &send_data, sizeof(send_data), 0);
            if(ret < 0){
                perror("send smoke data failed");
                // //lcd提示语
                // show_bmp("./bmp/connect_error.bmp", 50,200,280,30);
                // //显示一秒后删除
                // sleep(1);
                // lcd_draw_block(50,250, 280,30, 0x00ffffff);
            }
        }
        //显示数据，在显示数据前会判断数据有效性
        show_smoke_data(smoke_data, 195, 495, 16, 21, 0x00000000);

        //烟雾浓度超过15%提示超标文字
        if(smoke_data > 1500){
            //draw_words_row(300, 240, 32, 32*29/8, 10, smoke_outrange, 0x0000ff00);
            show_bmp("./bmp/somke_tip.bmp", 300, 270, 352, 60);
        }else{//未超标清楚提醒
            lcd_draw_block(300, 270, 352, 60, 0x00ffffff);
        }

        //点击返回按钮则退出本界面lcd_draw_block(360, 120, 50, 40, 0x000000ff);//退出
        if(position.y >120 && position.y < 160 &&position.x >360 && position.x<400 )//点击了退出
		{	
            //退出关闭socket描述符
            close(sock_fd);
			break;
		}
        sleep(1);
    }
}

/*
    功能：获取烟雾数据
    返回值：-1失败，其他成功
*/
int get_smoke_data()
{
    //打开设备文件
    int smoke_fd = open("/dev/ttySAC1", O_RDWR);
    if(smoke_fd < 0){
        perror("open /dev/ttySCA1 failed");
        return -1;
    }

    //发送读取指令到传感器文件
    unsigned char read_cmd[] = {
        0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79
    };
    int ret = write(smoke_fd, read_cmd, sizeof(read_cmd));
    if(ret < 0) {
        perror("write to ttySCA1 failed");
        return -1;
    }

    //读取数据
    unsigned char buf[9] = {0};
    ret = read(smoke_fd, buf, 9);
    if(ret < 0) {
        perror("read from ttySCA1 failed");
        return -1;
    }

    //数据处理
    

    close(smoke_fd);
    return (buf[2]<<8 | buf[3]);
}

/*
 * show_smoke_data()
 * 功能：在相应位置显示烟雾浓度
 * 参数1：烟雾浓度数据
 * 参数2：位置x行y列
 * 参数3：要显示数据单个数字的宽高
 * 参数3：颜色
 * 返回值：无
 */
void show_smoke_data(int smoke_data, int x, int y,int width, int height, int color)
{
    int data[6];
    //清屏
    for(int i = 0; i < 6; i++) {
        lcd_draw_block(x, y-i*13, width, height, 0x00FFFFFF);
    }

    //判断数据是否有效
    if(smoke_data < 0) {
        //数据错误显示全0
        memset(data, 0, sizeof(data));
    }else{//数据正常，处理数据
        data[0] = (smoke_data/10000)%10;
        data[1] = (smoke_data/1000)%10;
        data[2] = (smoke_data/100)%10;
        data[3] = 10;//显示小数点
        data[4] = (smoke_data/10)%10;
        data[5] = smoke_data%10;
    }
    //不显示多余的零，有效最高位
    int start;
    for(start = 0; start < 2; start++) {
        if(data[start] != 0) {
            break;
        }
    }
    //显示新的数据
    for(int i = 0; i < 6-start; i++) {
        draw_word(x, y-i*13, width, width*height/8, number+data[5-i]*42, color);
    }
}

#include "show_gy39.h"

/*
 * 显示温度湿度光照强度界面子程序
 * 显示温度湿度光照强度提示语
 * 展示温度湿度光照强度数据
 * 温度湿度光照强度过大提醒
 **/
void show_gy39()
{
    //清屏为白色 
	lcd_draw_block(0,0,800,480,0x00FFFFFF);
    //显示标题 环境参数
    //draw_words_row(150, 350, 32, 32*29/8, 4, environment_text, 0x00000000);
    //显示背景及文字
    // show_bmp("./pic/environment.bmp", 0, 0,800,480);
    show_bmp("./bmp/environment.bmp", (480-416)/2, (800-744)/2, 744, 416);
    // //显示返回图标
    // show_bmp("./pic/back.bmp", 400,500,40,40);

    //获取gy39数据
    struct gy39_data_t gy39_data;
    //每一秒获取并显示一次数据
    while(1) {
        //清楚上一次数据
        memset(&gy39_data, 0, sizeof(gy39_data));
        //获取数据
        int ret = get_GY39_data(&gy39_data);

        //连接云端成功发送数据，否则跳过
        if(sock_fd != -1){
            struct sock_data_t send_data;
            memset(&send_data, 0, sizeof(send_data));
            send_data.flag = 1;//标志位为1表示为温度/湿度/光照强度数据
            send_data.data[0] = gy39_data.temperature;
            send_data.data[1] = gy39_data.humidity;
            send_data.data[2] = gy39_data.illumination;
            //发送数据至服务器
            int ret = send(sock_fd, &send_data, sizeof(send_data), 0);
            if(ret < 0){
                perror("send gy39 data failed");
                // //lcd提示语
                // show_bmp("./bmp/connect_error.bmp", 50,200,280,30);
                // //显示一秒后删除
                // sleep(1);
                // lcd_draw_block(50,250, 280,30, 0x00ffffff);
            }
        }

        //展示数据
        show_gy39_data(&gy39_data);
        //温度光照强度过高提醒
        if(gy39_data.temperature>35 && gy39_data.illumination>350){
            //draw_words_row(350, 220, 32, 32*29/8, 13, temandill_outrange, 0x0000ff00);
            show_bmp("./bmp/weather.bmp", 350, 270, 304, 48);
        }else{//温度光照强度正常清楚提醒
            lcd_draw_block(350, 270, 304, 48, 0x00ffffff);
        }

        //点击退出按钮退出本界面
        if(position.y >120 && position.y < 160 &&position.x >360 && position.x<400 )//点击了退出
		{	
            //退出关闭socket描述符
            close(sock_fd);
			break;
		}
        //延时一秒
        sleep(1);
    }
}

/*
 * get_GY39_data()
 * 功能：获取gy39的数据
 * 参数：接受数据的数组指针
 * 返回值：-1失败，0成功
 */
int get_GY39_data(struct gy39_data_t *data)
{
    //打开设备文件
    int gy39_fd = open("/dev/ttySAC2", O_RDWR);
    if(gy39_fd < 0){
        perror("open gy39 failed");
        return -1;
    }

    int ret;
    //发送温湿度，海拔，气压读取指令
    unsigned char cmd[] = {0xA5, 0x52, 0xf7};
    ret = write(gy39_fd, cmd, 3);
    if (ret < 0){
        perror("write gy39 failed");
        return -1;
    }
    //读取数据
    unsigned char buf[20] = {0};
    ret = read(gy39_fd, buf, 20);
    if(ret < 0){
        perror("read from ttySCA2 failed");
        return -1;
    }
    //数据处理
    data->temperature = buf[4]<<8 | buf[5];
    data->airpressure = buf[6]<<24|buf[7]<<16|buf[8]<<8 | buf[9];
    data->humidity = buf[10]<<8 | buf[11];
    data->elevation = buf[12]<<8 | buf[13];

    //发送光照强度读取指令
    cmd[1] = 0x51;
    cmd[2] = 0xf6;
    ret = write(gy39_fd, cmd, 3);
    if (ret < 0){
        perror("write gy39 failed");
        return -1;
    }
    //读取数据
    memset(buf, 0, sizeof(buf));
    ret = read(gy39_fd, buf, 20);
    if(ret < 0){
        perror("read from ttySCA2 failed");
        return -1;
    }
    //处理数据
    data->illumination = buf[4]<<24|buf[5]<<16|buf[6]<<8|buf[7];

    //关闭gy39设备文件
    close(gy39_fd);
    return 0;
}

/*
 * show_one_data()
 * 功能：在相应位置显示数据
 * 参数1：数据值
 * 参数2：位置x行y列
 * 参数3：要显示数据单个数字的宽高
 * 参数3：颜色
 * 返回值：无
 */
void show_one_data(int gy39_data, int x, int y,int width, int height, int color)
{
    int  data[6];
    //处理数据
    data[0] = (gy39_data/100000)%10;
    data[1] = (gy39_data/10000)%10;
    data[2] = (gy39_data/1000)%10;
    data[3] = (gy39_data/100)%10;
    data[4] = (gy39_data/10)%10;
    data[5] = gy39_data%10;
    //不显示多余的零
    int start;
    for(start = 0; start < 6; start++) {
        if(data[start] != 0) {
            break;
        }
    }
    //清除上一次数据
    for(int i = 0; i < 5; i++) {
        lcd_draw_block(x, y-i*13, width, height, 0x00FFFFFF);
    }
    //显示新数据
    for(int i = 0; i < 6-start; i++) {
        draw_word(x, y-i*13, width, width*height/8, number+data[5-i]*42, color);
    }
}

/*
 * show_point_data()
 * 功能：在相应位置显示数据
 * 参数1：数据值
 * 参数2：位置x行y列
 * 参数3：要显示数据单个数字的宽高
 * 参数3：颜色
 * 返回值：无
 */
void show_point_data(int gy39_data, int x, int y,int width, int height, int color)
{
    int  data[6];
    //处理数据
    data[0] = (gy39_data/100000)%10;
    data[1] = (gy39_data/1000)%10;
    data[2] = (gy39_data/100)%10;
    data[3] = 10;
    data[4] = (gy39_data/10)%10;
    data[5] = gy39_data%10;
    //不显示多余的零
    int start;
    for(start = 0; start < 2; start++) {
        if(data[start] != 0) {
            break;
        }
    }
    //清除上一次数据
    for(int i = 0; i < 6; i++) {
        lcd_draw_block(x, y-i*13, width, height, 0x00FFFFFF);
    }
    //显示新数据
    for(int i = 0; i < 6-start; i++) {
        draw_word(x, y-i*13, width, width*height/8, number+data[5-i]*42, color);
    }
}


/*
 * show_gy39_data()
 * 显示温度/湿度/光照强度数据
 * 参数：数据结构体
 * 返回值：无
 */
void show_gy39_data(struct gy39_data_t *data)
{
    //显示温度数据+40 -35 -155
    show_one_data(data->temperature, 262, 220, 16, 21, 0x00000000);
    //显示湿度度数据
    show_point_data(data->humidity, 262, 415, 16, 21, 0x00000000);
    //显示光照强度数据
    show_one_data(data->illumination, 262, 565, 16, 21, 0x00000000);
}
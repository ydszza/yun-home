#include "show_contrlapp.h"

void sync_stat(void);

//定义空调/窗帘/灯默认状态为关闭
int flag_air=0;
int flag_curtain=0;
int flag_light=0;

//开关图片资源数组
char* bmp_name[] = {"./bmp/air_off.bmp","./bmp/air_on.bmp",
					"./bmp/curtain_off.bmp","./bmp/curtain_on.bmp",
					"./bmp/light_off.bmp","./bmp/light_on.bmp"};

/*
 * 显示控制家居界面子程序
 * 显示控制家居提示语
 * 展示控制家居开关
 **/
int show_contrlapp(void)
{
    //清屏为白色 
	lcd_draw_block(0,0,800,480,0x00FFFFFF);		
	//显示背景		
	show_bmp("./bmp/conrtlapp.bmp", (480-416)/2, (800-744)/2, 744, 416);
	// lcd_draw_block(200, 180, 80, 60, 0x000000ff);//退出
	// lcd_draw_block(200, 360, 80, 60, 0x000000ff);//退出
	// lcd_draw_block(200, 530, 80, 60, 0x000000ff);//退出
	//显示默认关闭空调图标	
	show_bmp(bmp_name[0], 200, 180, 80, 60);
	//show_bmp(500,200,40,40,bmp_name[3]);
	//显示默认关闭窗帘图标	
    show_bmp(bmp_name[2], 200, 360, 80, 60);
	//show_bmp(500,300,40,40,bmp_name[5]);
	//显示默认关闭灯图标	
    show_bmp(bmp_name[4], 200, 530, 80, 60);
	// //显示退出图标
    // show_bmp(bmp_name[6], 400,500,40,40);

	//开启一个线程同步家居状态并接收控制命令
	pthread_t thread;
    int ret = pthread_create(&thread, NULL, (void *)sync_stat, NULL);  

	//控制家居，频率一秒一次开关切换
	while(1)
	{
		//等待一秒
		sleep(1);
		//struct point start;
		//get_touch(&position);
        //printf("x=%d, y=%d\n", position.x, position.y);
		if (position.y >180 && position.y < 180+80 &&position.x >200 && position.x<200+60 )//空调状态切换
		{
			flag_air = !flag_air;
			show_bmp(bmp_name[flag_air], 200,180,80,60);
			//发送控制家电标志及初始家电状态
			struct sock_data_t send_data;
			send_data.flag = 2;/*同步家居状态标准位*/
			send_data.data[0] = flag_air;
			send_data.data[1] = flag_curtain;
			send_data.data[2] = flag_light;
			int ret = send(sock_fd, &send_data, sizeof(send_data), 0);
			if(ret < 0){
				perror("send smoke data failed");
			}
			continue;
		}
				
		else if(position.y >360 && position.y < 360+80 &&position.x >200 && position.x<260 )//窗帘状态切换
		{	
			flag_curtain = !flag_curtain;
			show_bmp(bmp_name[flag_curtain+2], 200,360,80,60);
			//发送控制家电标志及初始家电状态
			struct sock_data_t send_data;
			send_data.flag = 2;/*同步家居状态标准位*/
			send_data.data[0] = flag_air;
			send_data.data[1] = flag_curtain;
			send_data.data[2] = flag_light;
			int ret = send(sock_fd, &send_data, sizeof(send_data), 0);
			if(ret < 0){
				perror("send smoke data failed");
			}
			continue;
		}
		else if(position.y >500 && position.y < 530+80 &&position.x >200 && position.x<260 )//台灯状态切换
		{	
			flag_light = !flag_light;
			show_bmp(bmp_name[flag_light+4], 200, 530, 80, 60);
			//发送控制家电标志及初始家电状态
			struct sock_data_t send_data;
			send_data.flag = 2;/*同步家居状态标准位*/
			send_data.data[0] = flag_air;
			send_data.data[1] = flag_curtain;
			send_data.data[2] = flag_light;
			int ret = send(sock_fd, &send_data, sizeof(send_data), 0);
			if(ret < 0){
				perror("send smoke data failed");
			}
			continue;
		}
        else if(position.y >120 && position.y < 160 &&position.x >360 && position.x<400 )//退出本界面
		{	
			//取消线程
			pthread_cancel(thread);
			//关闭socket描述符
			close(sock_fd);
			break;
		}
		else//点击其他位置无效
		{
		}	
	}
	return 0;
}

/**
 * 功能：与服务器同步家居状态及控制
 * 参数：socket描述符
 * 返回值：无
*/
void sync_stat(void)
{
	//循环接收与发送家居状态
	while(1) {
		//发送控制家电标志及家电状态
		struct sock_data_t send_data;
		send_data.flag = 2;/*同步家居状态标准位*/
		send_data.data[0] = flag_air;
		send_data.data[1] = flag_curtain;
		send_data.data[2] = flag_light;
		int ret = send(sock_fd, &send_data, sizeof(send_data), 0);
		if(ret < 0){
			perror("send smoke data failed");
			//lcd提示语
			// show_bmp("./bmp/connect_error.bmp", 50,200,280,30);
			// //显示一秒后删除
			// sleep(1);
			// lcd_draw_block(50,250, 280,30, 0x00ffffff);
		}

		//接收服务器命令
		unsigned char cmd_buf[4];
		ret = recv(sock_fd, cmd_buf, sizeof(cmd_buf), 0);
		if(ret < 0){
			perror("recv cmd");
        	//lcd提示语
        	// show_bmp("./bmp/connect_error.bmp", 50,200,280,30);
			//  //显示一秒后删除
        	// sleep(1);
        	// lcd_draw_block(50,250, 280,30, 0x00ffffff);
		}
		int *cmd = (int *)cmd_buf;
		//修改本地状态
		switch(*cmd){
			case 0: flag_air = !flag_air; 
					show_bmp(bmp_name[flag_air], 200,180,80,60);
			 		break;
			case 1: flag_curtain = !flag_curtain; 
					show_bmp(bmp_name[flag_curtain+2], 200,360,80,60);
					break;
			case 2: flag_light = !flag_light; 
					show_bmp(bmp_name[flag_light+4], 200,530,80,60);
					break;
			default: break;
		}
	}
}
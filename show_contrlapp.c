#include "show_contrlapp.h"

/*
 * 显示控制家居界面子程序
 * 显示控制家居提示语
 * 展示控制家居开关
 **/
int show_contrlapp(void)
{
    //清屏为白色 
	lcd_draw_block(0,0,800,480,0x00FFFFFF);
	//开关图片资源数组
	char* bmp_name[] = {"./pic/air_open.bmp","./pic/air_close.bmp",
                        "./pic/curtain_open.bmp","./pic/curtain_close.bmp",
                        "./pic/light_open.bmp","./pic/light_close.bmp",
                        "./pic/back.bmp",};
	//显示默认关闭空调图标					
	show_bmp(bmp_name[1], 100,500,40,40);
	//show_bmp(500,200,40,40,bmp_name[3]);
	//显示默认关闭窗帘图标	
    show_bmp(bmp_name[3], 200,500,40,40);
	//show_bmp(500,300,40,40,bmp_name[5]);
	//显示默认关闭灯图标	
    show_bmp(bmp_name[5], 300,500,40,40);
	//显示退出图标
    show_bmp(bmp_name[6], 400,500,40,40);

	//定义空调/窗帘/灯默认状态为关闭
	int flag_light=0;
	int flag_curtain=0;
	int flag_air=0;

	//控制家居，频率一秒一次开关切换
	while(1)
	{
		//等待一秒
		sleep(1);
		//struct point start;
		//get_touch(&position);
        //printf("x=%d, y=%d\n", position.x, position.y);
		if (position.y >500 && position.y < 540 &&position.x >100 && position.x<140 )//空调状态切换
		{
			if(flag_air==0)
			{
				//lcd_draw_block(500,100,40,40,0x00FFFFFF);//x是宽
				//show_bmp(500,100,40,40,bmp_name[0]);
                show_bmp(bmp_name[0], 100,500,40,40);
				//printf("显示打开空调图片\n");
				flag_air=1;
				continue;
			}
			if(flag_air==1)
			{
				//lcd_draw_block(500,100,40,40,0x00FFFFFF);
				
				//show_bmp(500,100,40,40,bmp_name[1]);
                show_bmp(bmp_name[1], 100,500,40,40);
				//printf("显示关闭空调图片\n");
				flag_air=0;
				continue;
			}
		}
				
		else if(position.y >500 && position.y < 540 &&position.x >200 && position.x<240 )//窗帘状态切换
		{	
			if(flag_curtain==0)
			{
				//lcd_draw_block(500,200,40,40,0x00FFFFFF);
				//show_bmp(500,200,40,40,bmp_name[2]);
                show_bmp(bmp_name[2], 200,500,40,40);
				//printf("显示打开窗帘图片\n");
				flag_curtain=1;
				continue;
			}
			else
			{	
				//lcd_draw_block(500,200,40,40,0x00FFFFFF);
				//show_bmp(500,200,40,40,bmp_name[3]);
                show_bmp(bmp_name[3], 200,500,40,40);
				//printf("显示关闭窗帘图片\n");
				flag_curtain=0;
				continue;
			}
		}
		else if(position.y >500 && position.y < 540 &&position.x >300 && position.x<340 )//台灯状态切换
		{	
			if(flag_light==0)
			{
				//lcd_draw_block(500,300,40,40,0x00FFFFFF);
				//show_bmp(500,300,40,40,bmp_name[4]);
                show_bmp(bmp_name[4], 300,500,40,40);
				//printf("显示打开台灯图片\n");
				flag_light=1;
				continue;
			}
			else
			{
				//lcd_draw_block(500,300,40,40,0x00FFFFFF);
				//show_bmp(500,300,40,40,bmp_name[5]);
                show_bmp(bmp_name[5], 300,500,40,40);
				////printf("显示关闭窗帘图片\n");
				flag_light=0;
				continue;
			}
		}
        else if(position.y >500 && position.y < 540 &&position.x >400 && position.x<440 )//退出本界面
		{	
			break;
		}
		else//点击其他位置无效
		{
		}	
	}
	return 0;
}
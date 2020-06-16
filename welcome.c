#include "welcome.h"


/**
 * 欢迎界面子程序，
 * 显示提示语并等待选择要进入的界面
 */
void show_welcome(void)
{
    //显示欢迎界面提示语及功能选择图标
    show_bmp("./pic/home.bmp", 0, 0, 800, 480);
    //显示退出图标
    show_bmp("./pic/back.bmp", 400,400,40,40);
    //初始化页面选择标志
    page_index = 0;
    // lcd_draw_block(260, 76, 90, 50, 0x000000ff);
    // lcd_draw_block(260, 310, 90, 50, 0x000000ff);
    // lcd_draw_block(260, 560, 90, 50, 0x000000ff);
    //等待选择功能
    while(1){ 
        if(position.x>260&&position.x<260+50&&position.y>76&&position.y<76+90){//选择查看烟雾浓度界面
            page_index = 1;
            break;
        }else if(position.x>260&&position.x<260+50&&position.y>310&&position.y<310+90){//选择查看温度湿度光照强度界面
            page_index = 2;
            break;
        }else if(position.x>260&&position.x<260+50&&position.y>560&&position.y<560+90){//选择进入家居控制界面
            page_index = 3;
            break;
        }else if(position.y >400 && position.y < 440 &&position.x >400 && position.x<440 )//选择退出程序
		{	
            page_index = 4;
			break;
		}else{//点击其他位置无效什么也不做
        }
    }
    // //显示提示  欢迎进入智能家居系统
    // draw_words_row(150, 150, 40, 40*35/8, 10, welcome_text, WELCOME_COLOR);

    // //显示烟雾浓度查看选择
    // draw_words_row(190, 150, 32, 32*29/8, 4, smoke_text, WELCOME_COLOR);
    // show_button(190, 150+32*5, 55, 28, 0x000000ff);

    // //显示环境参数查看选择
    // draw_words_row(220, 150, 32, 32*29/8, 4, environment_text, WELCOME_COLOR);
    // show_button(220, 150+32*5, 55, 28, 0x000000ff);

    // //显示家居控制查看选择
    // draw_words_row(250, 150, 32, 32*29/8, 4, control_app_text, WELCOME_COLOR);
    // show_button(250, 150+32*5, 55, 28, 0x000000ff);
}
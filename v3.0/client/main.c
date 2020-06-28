#include "words.h"
#include "welcome.h"
#include "show_smoke.h"
#include "show_gy39.h"
#include "show_contrlapp.h"
#include <pthread.h>


int lcd_fd;//屏幕设备文件描述符
int *plcd;//屏幕设备文件内存映射800*480
int sock_fd;//连接服务器套接字

struct point position;//鼠标点击坐标结果提
int page_index;//页面选择

/* 
 * 获取鼠标点击坐标线程函数
 * 每一秒钟获取一次鼠标点击坐标
 * */
void get_position(void)
{
    while(1) {
        //读取坐标
        int ret = get_touch(&position);
        if(ret < 0){
            printf("get_touch failed\n");
            return ;
        }
        //延时一秒
        sleep(1);
        //printf("x= %d, y= %d", position.x, position.y);
    }
}

int main(void)
{
    //初始化屏幕 
	lcd_init();

	//清屏为白色 
	lcd_draw_block(0,0,800,480,0x00FFFFFF);

    //创建多线程读取点击坐标
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, (void *)get_position, NULL);  
    
    //默认套接字无效
    sock_fd = -1;

    //进入程序功能
    while(1) {
        //展示首页欢迎界面，
        //等待选择页面
        show_welcome(); 

        //从欢迎主界面退出
        //判断进入了哪个页面 
        switch(page_index){
            case 1: show_smoke_page();//进入显示烟雾浓度界面
                    break;
            case 2: show_gy39();//进入显示温度湿度光照强度界面
                    break;
            case 3: show_contrlapp();//进入家居控制界面
                    break;
            case 4: lcd_draw_block(0,0,800,480,0x00FFFFFF);//从主界面点击退出，清屏为白色并退出程序
                    lcd_uninit();
                    return 0;
            default: break;
        }
        //切换界面清除鼠标点击事件，防止误判上次点击在本次执行
        position.x = position.y = -1;
    }
    
    //展示烟雾浓度数据
    //show_smoke_page();

    //展示环境参数页面
    //show_gy39();

    //展示家居开关界面
    //show_contrlapp();

	//解除初始化 
	lcd_uninit();
    close(sock_fd);
    return 0;
}
#ifndef _SHOW_GY39_H
#define _SHOW_GY39_H

#include "lcd.h"
#include "socket.h"



//引用main中定义的全局变量
//extern char environment_text[];
extern char number[];
//extern char temandill_outrange[];
extern int lcd_fd;
extern int *plcd;
extern int sock_fd;
extern struct point position;

//定义接收gy39数据的结构体
struct gy39_data_t {
    int illumination;//光照强度
    int temperature;//温度
    int airpressure;//大气压
    int humidity;//湿度
    int elevation;//海拔
};


void show_gy39();
int get_GY39_data(struct gy39_data_t *data);
void show_one_data(int gy39_data, int x, int y,int width, int height, int color);
void show_gy39_data(struct gy39_data_t *data);


#endif // !_SHOW_GY39_H
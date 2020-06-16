#ifndef __WELCOME_H
#define __WELCOME_H

#include "lcd.h"

//引用main文件中的全局变量
extern int *plcd;
extern int lcd_fd;
extern struct point position;
extern int page_index;
// extern char welcome_text[];
// extern char smoke_text[];
// extern char environment_text[];
// extern char control_app_text[];

//提示语文字颜色
// #define WELCOME_COLOR   0x00FF0000



void show_welcome(void);


#endif // !__WELCOME_H
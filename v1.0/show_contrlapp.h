#ifndef __SHOW_CONTRLAPP_H
#define __SHOW_CONTRLAPP_H

#include "lcd.h"

//引用main中定义的全局变量
extern int *plcd;
extern int lcd_fd;
extern struct point position;


int show_contrlapp(void);




#endif // !__SHOW_CONTRLAPP_H
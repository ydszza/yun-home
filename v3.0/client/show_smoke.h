#ifndef __SHOW_SMOKE_H
#define __SHOW_SMOKE_H

#include "lcd.h"
#include "socket.h"




extern int *plcd;
extern int lcd_fd;
extern int sock_fd;
extern struct point position;
//extern char smoke_text[];
extern char number[];
//extern char smoke_outrange[];


void show_smoke_page(void);
int get_smoke_data();
void show_smoke_data(int smoke_data, int x, int y,int width, int height, int color);

#endif // !__SHOW_SMOKE_H
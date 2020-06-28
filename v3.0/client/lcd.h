#ifndef __LCD_H
#define __LCD_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>


struct point{
    int x;
    int y;
};

extern int *plcd;
extern int lcd_fd;

int lcd_init(void);
int lcd_uninit(void);
void lcd_draw_point(int x,int y,int color);
void lcd_draw_block(int x0,int y0,int width,int high,int color);
void draw_word(int x0,int y0,int width,int size,char word_name[],int color);
void draw_words_row(int x0, int y0, int width, int size, int cnt, char words[], int color);
void draw_words_column(int x0, int y0, int width, int size, int cnt, char words[], int color);
int show_bmp(char *pathname, int x0, int y0, int width, int height);
int get_touch(struct point *position);






#endif // !__LCD_H
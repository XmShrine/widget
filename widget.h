#ifndef widget_h
#define widget_h
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "array.h"
struct widget {
    int bx;
    int by;
    int lx;
    int ly;
    char* content;
};
struct widget_box {
    char* uu;
    char* dd;
    char* ll;
    char* rr;
    char* lu;
    char* ld;
    char* ru;
    char* rd;
};
struct widget_content_format {
    int gp; // 0 1 2 ^ - v
    int wp; // 0 1 2 [ | ]
};
array* scrstr;
int scr_init(void);
int scr_drop(void);
int scr_flush(void);
int scr_getch(void);
struct winsize scr_winsize(void);
struct widget scr_std(void);
array* fcolor(int c);
array* frcolor(array* c);
array* fmove(int x, int y);
array* fcolor(int c);
array* frcolor(array* c);
array* fmove(int x, int y);
array* widget_fill(struct widget wid, char* c);
array* widget_box(struct widget wid, struct widget_box box);
array* widget_content(struct widget wid, struct widget_content_format format);
#endif
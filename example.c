#include "widget.h"
// 注意，这个库需要使用 array 库，也可在 github 仓库找到，这个库与 array 不同，只在 MacOS, Unix, Linux 或类似系统生效，Windows 请另寻他库
int main() {
    struct widget wid = {4/*begin_x*/, 3/*begin_y*/, 25/*col*/, 8/*row*/, "This is a text. Welcome to use widget.c. Hope you have fun."};
    struct widget_box box = {"1","2","3","4","5","6","7","8"};
    struct widget_content_format fmt = {0,0};
    scr_init(); // 使用前开启
    widget_fill(scr_std(), " ");
    widget_box(wid, box); // 如显示效果
    wid.bx ++; // 4
    wid.by ++; // 3
    wid.lx -= 2; // 15
    wid.ly -= 2; // 4
    widget_content(wid, fmt); // 如显示效果
    scr_flush(); // 刷新才会显示
    scr_getch();
    scr_drop(); // 使用后关闭
    wid.bx --; // 4
    wid.by --; // 3
    wid.lx += 2; // 15
    wid.ly += 2; // 4
    scr_init(); // 使用前开启
    widget_fill(scr_std(), " ");
    widget_box(wid, box); // 如显示效果
    wid.bx ++; // 4
    wid.by ++; // 3
    wid.lx -= 2; // 15
    wid.ly -= 2; // 4
    scr_flush();
    scr_getch();
    widget_content(wid, fmt); // 如显示效果
    scr_flush(); // 刷新才会显示
    scr_getch();
    scr_drop(); // 使用后关闭
    // 请注意 flush 用的地方不一样会导致怎样不一样的结果
}
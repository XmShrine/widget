#include "widget.h"
int print_char_array(array* arr) {
    if (!arr || array_is_empty(arr) || array_size(arr)!=sizeof(char)) {
        return -1;
    }
    for (size_t i=0; i<array_len(arr); i++) {
        printf("%c", array_fetch_deref(arr, i, char));
    }
    return 0;
}
int print_int_array(array* arr) {
    if (!arr || array_is_empty(arr) || array_size(arr)!=sizeof(int)) {
        return -1;
    }
    printf("[");
    for (size_t i=0; i<array_len(arr); i++) {
        printf("%d,", array_fetch_deref(arr, i, int));
    }
    printf("\b \b]\n");
    return 0;
}
array* scrstr;
int scr_init(void) {
    scrstr = array_init(sizeof(char));
    if (!scrstr) {
        return -1;
    }
    return 0;
}
int scr_drop(void) {
    return array_drop(&scrstr);
}
int scr_flush(void) {
    print_char_array(scrstr);
    array_drop(&scrstr);
    scrstr = array_init(sizeof(char));
    return 0;
}
int scr_getch(void) {
    fflush(0);
    struct termios tm, tm_old;
    int fd = 0, ch;
    if (tcgetattr(fd, &tm) < 0) return -1;
    tm_old = tm;
    cfmakeraw(&tm);
    if (tcsetattr(fd, TCSANOW, &tm) < 0) return -1;
    ch = getchar();
    if (tcsetattr(fd, TCSANOW, &tm_old) < 0) return -1;
    fflush(0);
    return ch;
}
struct winsize scr_winsize(void) {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size;
}
struct widget scr_std(void) {
    struct widget wid;
    wid.bx = wid.by = 1;
    wid.lx = scr_winsize().ws_col;
    wid.ly = scr_winsize().ws_row;
    return wid;
}
array* fcolor(int c) {
    array* arr = array_init(sizeof(char));
    if (!arr) {
        return NULL;
    }
    array_add_back(arr, snprintf(0, 0, "\033[%dm", c));
    snprintf(array_fetch_front(arr), array_len(arr)+1, "\033[%dm", c);
    return arr;
}
array* frcolor(array* c) {
    if (!c || array_is_empty(c))  {
        return NULL;
    }
    array* arr = array_init(sizeof(char));
    for (size_t i=0; i<array_len(c); i++) {
        array_append(arr, fcolor(array_fetch_deref(c, i, int)));
    }
    return arr;
}
array* fmove(int x, int y) {
    array* arr = array_init(sizeof(char));
    if (!arr) {
        return NULL;
    }
    array_add_back(arr, snprintf(0, 0, "\033[%d;%dH", y, x));
    snprintf(array_fetch_front(arr), array_len(arr)+1, "\033[%d;%dH", y, x);
    return arr;
}
array* widget_fill(struct widget wid, char* c) {
    if (!c) {
        return NULL;
    }
    array* arr = array_init(sizeof(char));
    if (!arr) {
        return NULL;
    }
    for (int y=0; y<wid.ly; y++) {
        for (int x=0; x<wid.lx; x++) {
            array* temp = fmove(wid.bx+x, wid.by+y);
            array* cstr = array_from_nar(c, sizeof(char), strlen(c));
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);
        }
    }
    array_append(scrstr, arr);
    return arr;
}
array* widget_box(struct widget wid, struct widget_box box) {
    array* arr = array_init(sizeof(char));
    if (!arr) {
        return NULL;
    } 
    if (box.uu) {
        for (int i=0; i<wid.lx-2; i++) {
            array* temp = fmove(wid.bx+i+1, wid.by);
            array* cstr = array_from_nar(box.uu, sizeof(char), strlen(box.uu));
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);
        }
    }
    if (box.dd) {
        for (int i=0; i<wid.lx-2; i++) {
            array* temp = fmove(wid.bx+i+1, wid.by+wid.ly-1);
            array* cstr = array_from_nar(box.dd, sizeof(char), strlen(box.dd));
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);
        }
    }
    if (box.ll) {
        for (int i=0; i<wid.ly-2; i++) {
            array* temp = fmove(wid.bx, wid.by+i+1);
            array* cstr = array_from_nar(box.ll, sizeof(char), strlen(box.ll));
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);
        }
    }
    if (box.rr) {
        for (int i=0; i<wid.ly-2; i++) {
            array* temp = fmove(wid.bx+wid.lx-1, wid.by+i+1);
            array* cstr = array_from_nar(box.rr, sizeof(char), strlen(box.rr));
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);
        }
    }
    if (box.lu) {
        array* temp = fmove(wid.bx, wid.by);
        array* cstr = array_from_nar(box.lu, sizeof(char), strlen(box.lu));
        if (!temp || !cstr) {
            return NULL;
        }
        array_append(arr, temp);
        array_append(arr, cstr);
        array_drop(&temp);
        array_drop(&cstr);
    }
    if (box.ld) {
        array* temp = fmove(wid.bx, wid.by+wid.ly-1);
        array* cstr = array_from_nar(box.ld, sizeof(char), strlen(box.ld));
        if (!temp || !cstr) {
            return NULL;
        }
        array_append(arr, temp);
        array_append(arr, cstr);
        array_drop(&temp);
        array_drop(&cstr);
    }
    if (box.ru) {
        array* temp = fmove(wid.bx+wid.lx-1, wid.by);
        array* cstr = array_from_nar(box.ru, sizeof(char), strlen(box.ru));
        if (!temp || !cstr) {
            return NULL;
        }
        array_append(arr, temp);
        array_append(arr, cstr);
        array_drop(&temp);
        array_drop(&cstr);
    }
    if (box.rd) {
        array* temp = fmove(wid.bx+wid.lx-1, wid.by+wid.ly-1);
        array* cstr = array_from_nar(box.rd, sizeof(char), strlen(box.rd));
        if (!temp || !cstr) {
            return NULL;
        }
        array_append(arr, temp);
        array_append(arr, cstr);
        array_drop(&temp);
        array_drop(&cstr);
    }
    array_append(scrstr, arr);
    return arr;
}
array* widget_content(struct widget wid, struct widget_content_format format) {
    if (!wid.content) {
        return NULL;
    }
    array* arr = array_init(sizeof(char));
    array* str = array_from_nar(wid.content, sizeof(char), strlen(wid.content));
    if (!arr | !str) {
        return NULL;
    }
    int cstr_max_len = wid.lx*wid.ly;
    if (cstr_max_len>array_len(str)) {
        cstr_max_len = array_len(str);
    }
    int cstr_line_num = (cstr_max_len/wid.lx)+((cstr_max_len%wid.lx)>0);
    int space = 0;
    int enter = 0;
    if (format.gp==1) {
        enter = (wid.ly-cstr_line_num)/2;
    }
    if (format.gp==2) {
        enter = (wid.ly-cstr_line_num);
    }
    for (int i=0; i<cstr_line_num; i++) {
        if (i<cstr_line_num-1) {
            space = 0;
            array* temp = fmove(wid.bx+space, wid.by+enter+i);
            array* cstr = array_from_nar(array_fetch(str, i*wid.lx), sizeof(char), wid.lx);
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);

        } else {
            space = 0;
            if (format.wp==1) space = (wid.lx-(cstr_max_len-i*wid.lx))/2;
            if (format.wp==2) space = (wid.lx-(cstr_max_len-i*wid.lx));
            array* temp = fmove(wid.bx+space, wid.by+enter+i);
            array* cstr = array_from_nar(array_fetch(str, i*wid.lx), sizeof(char), cstr_max_len-i*wid.lx);
            if (!temp || !cstr) {
                return NULL;
            }
            array_append(arr, temp);
            array_append(arr, cstr);
            array_drop(&temp);
            array_drop(&cstr);
        }
    }
    array_append(scrstr, arr);
    return arr;
}
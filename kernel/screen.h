#ifndef SCREEN_H
#define SCREEN_H

void clear_screen();
void print(const char *str);
void print_char(char c);
void print_int(int n);
void print_hex(unsigned int n);
void set_color(unsigned char color);

/* VGA colors */
#define COLOR_BLACK   0
#define COLOR_BLUE    1
#define COLOR_GREEN   2
#define COLOR_CYAN    3
#define COLOR_RED     4
#define COLOR_MAGENTA 5
#define COLOR_BROWN   6
#define COLOR_WHITE   7
#define COLOR_GRAY    8
#define COLOR_LBLUE   9
#define COLOR_LGREEN  10
#define COLOR_LCYAN   11
#define COLOR_LRED    12
#define COLOR_LMAG    13
#define COLOR_YELLOW  14
#define COLOR_BWHITE  15

#define MAKE_COLOR(fg,bg) ((bg<<4)|fg)

#endif

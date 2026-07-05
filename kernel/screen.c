#include "screen.h"
#include "ports.h"

#define VIDEO_MEMORY ((volatile unsigned short*)0xB8000)
#define COLS 80
#define ROWS 25

static int cursorX = 0;
static int cursorY = 0;
static unsigned char cur_color = MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK);

static void update_hw_cursor() {
    unsigned short pos = cursorY * COLS + cursorX;
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

static void scroll() {
    unsigned short blank = (cur_color << 8) | ' ';
    if (cursorY >= ROWS) {
        for (int y = 0; y < ROWS - 1; y++)
            for (int x = 0; x < COLS; x++)
                VIDEO_MEMORY[y * COLS + x] = VIDEO_MEMORY[(y+1) * COLS + x];
        for (int x = 0; x < COLS; x++)
            VIDEO_MEMORY[(ROWS-1) * COLS + x] = blank;
        cursorY = ROWS - 1;
    }
}

void set_color(unsigned char color) {
    cur_color = color;
}

void print_char(char c) {
    if (c == '\n') {
        cursorX = 0;
        cursorY++;
    } else if (c == '\r') {
        cursorX = 0;
    } else if (c == '\b') {
        if (cursorX > 0) {
            cursorX--;
            VIDEO_MEMORY[cursorY * COLS + cursorX] = (cur_color << 8) | ' ';
        }
    } else if (c == '\t') {
        cursorX = (cursorX + 4) & ~3;
        if (cursorX >= COLS) { cursorX = 0; cursorY++; }
    } else {
        VIDEO_MEMORY[cursorY * COLS + cursorX] = (cur_color << 8) | (unsigned char)c;
        cursorX++;
        if (cursorX >= COLS) { cursorX = 0; cursorY++; }
    }
    scroll();
    update_hw_cursor();
}

void print(const char *str) {
    while (*str) print_char(*str++);
}

void clear_screen() {
    unsigned short blank = (MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK) << 8) | ' ';
    for (int i = 0; i < COLS * ROWS; i++)
        VIDEO_MEMORY[i] = blank;
    cursorX = 0; cursorY = 0;
    cur_color = MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK);
    update_hw_cursor();
}

void print_int(int n) {
    if (n < 0) { print_char('-'); n = -n; }
    if (n >= 10) print_int(n / 10);
    print_char('0' + (n % 10));
}

void print_hex(unsigned int n) {
    const char *hex = "0123456789ABCDEF";
    print("0x");
    for (int i = 28; i >= 0; i -= 4)
        print_char(hex[(n >> i) & 0xF]);
}

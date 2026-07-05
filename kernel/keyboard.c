#include "keyboard.h"
#include "ports.h"

/* PS/2 scancode set 1 to ASCII (partial, lowercase) */
static const char scancode_ascii[128] = {
    0,   27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,   'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,   '\\','z','x','c','v','b','n','m',',','.','/',0,
    '*', 0,' ',
    0,0,0,0,0,0,0,0,0,0,0,0,  /* F1-F12 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
static const char scancode_shift[128] = {
    0,   27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,   'A','S','D','F','G','H','J','K','L',':','"','~',
    0,   '|','Z','X','C','V','B','N','M','<','>','?',0,
    '*', 0,' ',
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define KBD_BUF 256
static char kbd_buf[KBD_BUF];
static volatile int kbd_head = 0;
static volatile int kbd_tail = 0;
static int shift_down = 0;

/* PIC remapping constants */
#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI   0x20

static void pic_remap() {
    outb(PIC1_CMD,  0x11); outb(PIC2_CMD,  0x11);
    outb(PIC1_DATA, 0x20); outb(PIC2_DATA, 0x28); /* remap to 0x20-0x2F */
    outb(PIC1_DATA, 0x04); outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01); outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0xFD); /* enable IRQ1 (keyboard) only */
    outb(PIC2_DATA, 0xFF);
}

void keyboard_handler() {
    unsigned char sc = inb(0x60);
    if (sc & 0x80) {
        /* key release */
        sc &= 0x7F;
        if (sc == 0x2A || sc == 0x36) shift_down = 0;
    } else {
        if (sc == 0x2A || sc == 0x36) { shift_down = 1; }
        else {
            char c = shift_down ? scancode_shift[sc] : scancode_ascii[sc];
            if (c) {
                kbd_buf[kbd_head % KBD_BUF] = c;
                kbd_head++;
            }
        }
    }
    outb(PIC1_CMD, PIC_EOI);
}

void keyboard_init() {
    pic_remap();
    /* flush keyboard buffer */
    while (inb(0x64) & 1) inb(0x60);
}

char keyboard_getchar() {
    while (kbd_head == kbd_tail) {
        __asm__ volatile ("sti; hlt; cli");
    }
    char c = kbd_buf[kbd_tail % KBD_BUF];
    kbd_tail++;
    return c;
}

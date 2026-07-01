#include "idt.h"
#include "keyboard.h"

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char  always0;
    unsigned char  flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo  = base & 0xFFFF;
    idt[num].base_hi  = (base >> 16) & 0xFFFF;
    idt[num].sel      = sel;
    idt[num].always0  = 0;
    idt[num].flags    = flags;
}

/* ISR stub for IRQ1 (keyboard) — calls keyboard_handler() */
extern void irq1_stub();

void idt_init() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (unsigned int)&idt;

    /* zero all entries */
    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0, 0, 0);

    /* IRQ1 = int 0x21 after PIC remap */
    idt_set_gate(0x21, (unsigned int)irq1_stub, 0x08, 0x8E);

    __asm__ volatile ("lidt (%0)" : : "r"(&idtp));
    __asm__ volatile ("sti");
}

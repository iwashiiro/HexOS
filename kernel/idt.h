#ifndef IDT_H
#define IDT_H

struct regs {
    unsigned int edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax; /* pushed by pusha */
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags;
};

void idt_init();

#endif

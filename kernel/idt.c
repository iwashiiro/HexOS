#include "idt.h"
#include "screen.h"

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

extern void isr0();  extern void isr1();  extern void isr2();  extern void isr3();
extern void isr4();  extern void isr5();  extern void isr6();  extern void isr7();
extern void isr8();  extern void isr9();  extern void isr10(); extern void isr11();
extern void isr12(); extern void isr13(); extern void isr14(); extern void isr15();
extern void isr16(); extern void isr17(); extern void isr18(); extern void isr19();
extern void isr20(); extern void isr21(); extern void isr22(); extern void isr23();
extern void isr24(); extern void isr25(); extern void isr26(); extern void isr27();
extern void isr28(); extern void isr29(); extern void isr30(); extern void isr31();

extern void irq1_stub();

static const char *exception_messages[32] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
    "Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved"
};


/* Handles CPU exceptions by printing diagnostic info and halting,
 * this avoids a silent triple fault and reset on an unhandled
 * exception. */
void isr_handler(struct regs *r) {
    set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
    print("\n*** KERNEL PANIC ***\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));

    print("Exception #");
    print_int((int)r->int_no);
    print(": ");
    if (r->int_no < 32)
        print(exception_messages[r->int_no]);
    print("\n");

    print("Error code: ");
    print_hex(r->err_code);
    print("\n");

    print("EIP: ");
    print_hex(r->eip);
    print("  CS: ");
    print_hex(r->cs);
    print("  EFLAGS: ");
    print_hex(r->eflags);
    print("\n");

    print("System halted.\n");

    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void idt_init() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (unsigned int)&idt;

    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0, 0, 0);

    /* CPU exceptions 0-31 */
    idt_set_gate(0,  (unsigned int)isr0,  0x10, 0x8E);
    idt_set_gate(1,  (unsigned int)isr1,  0x10, 0x8E);
    idt_set_gate(2,  (unsigned int)isr2,  0x10, 0x8E);
    idt_set_gate(3,  (unsigned int)isr3,  0x10, 0x8E);
    idt_set_gate(4,  (unsigned int)isr4,  0x10, 0x8E);
    idt_set_gate(5,  (unsigned int)isr5,  0x10, 0x8E);
    idt_set_gate(6,  (unsigned int)isr6,  0x10, 0x8E);
    idt_set_gate(7,  (unsigned int)isr7,  0x10, 0x8E);
    idt_set_gate(8,  (unsigned int)isr8,  0x10, 0x8E);
    idt_set_gate(9,  (unsigned int)isr9,  0x10, 0x8E);
    idt_set_gate(10, (unsigned int)isr10, 0x10, 0x8E);
    idt_set_gate(11, (unsigned int)isr11, 0x10, 0x8E);
    idt_set_gate(12, (unsigned int)isr12, 0x10, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x10, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x10, 0x8E);
    idt_set_gate(15, (unsigned int)isr15, 0x10, 0x8E);
    idt_set_gate(16, (unsigned int)isr16, 0x10, 0x8E);
    idt_set_gate(17, (unsigned int)isr17, 0x10, 0x8E);
    idt_set_gate(18, (unsigned int)isr18, 0x10, 0x8E);
    idt_set_gate(19, (unsigned int)isr19, 0x10, 0x8E);
    idt_set_gate(20, (unsigned int)isr20, 0x10, 0x8E);
    idt_set_gate(21, (unsigned int)isr21, 0x10, 0x8E);
    idt_set_gate(22, (unsigned int)isr22, 0x10, 0x8E);
    idt_set_gate(23, (unsigned int)isr23, 0x10, 0x8E);
    idt_set_gate(24, (unsigned int)isr24, 0x10, 0x8E);
    idt_set_gate(25, (unsigned int)isr25, 0x10, 0x8E);
    idt_set_gate(26, (unsigned int)isr26, 0x10, 0x8E);
    idt_set_gate(27, (unsigned int)isr27, 0x10, 0x8E);
    idt_set_gate(28, (unsigned int)isr28, 0x10, 0x8E);
    idt_set_gate(29, (unsigned int)isr29, 0x10, 0x8E);
    idt_set_gate(30, (unsigned int)isr30, 0x10, 0x8E);
    idt_set_gate(31, (unsigned int)isr31, 0x10, 0x8E);

    idt_set_gate(0x21, (unsigned int)irq1_stub, 0x10, 0x8E);

    __asm__ volatile ("lidt (%0)" : : "r"(&idtp));
    __asm__ volatile ("sti");
}

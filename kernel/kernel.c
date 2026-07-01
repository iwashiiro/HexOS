#include "screen.h"
#include "memory.h"
#include "keyboard.h"
#include "idt.h"
#include "shell.h"

void kernel_main() {
    clear_screen();

    /* Banner */
    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("  ==========================================\n");
    print("              HexOS_!  v0.2.0              \n");
    print("  ==========================================\n");
    set_color(MAKE_COLOR(COLOR_LBLUE, COLOR_BLACK));
    print("  Built from scratch — Linux From Scratch   \n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("\n");

    /* Init */
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("Memory subsystem initialized\n");

    memory_init();

    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("IDT & interrupts configured\n");

    idt_init();

    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("PS/2 keyboard ready (IRQ1)\n");

    keyboard_init();

    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("HexShell starting...\n");

    shell_start();
}

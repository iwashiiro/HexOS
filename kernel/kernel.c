#include "screen.h"
#include "memory.h"
#include "keyboard.h"
#include "idt.h"
#include "shell.h"
#include "auth.h"
#include "security.h"

void kernel_main() {
    clear_screen();

    /* Banner */
    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("  ==========================================\n");
    print("              HexOS_!  v0.2.0              \n");
    print("  ==========================================\n");
    set_color(MAKE_COLOR(COLOR_LBLUE, COLOR_BLACK));
    print("  A small security-focused OS, built from scratch\n");
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
    print("IDT and interrupts configured\n");

    idt_init();

    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("PS/2 keyboard ready (IRQ1)\n");

    keyboard_init();

    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("Security subsystem initialized\n");

    security_baseline_init();

    print("\n");
    auth_login();

    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("[ OK ] ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("HexShell starting...\n");

    shell_start();
}

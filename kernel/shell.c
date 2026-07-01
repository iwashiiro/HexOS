#include "screen.h"
#include "string.h"
#include "keyboard.h"
#include "shell.h"
#include "memory.h"

#define CMD_LEN  128
#define MAX_ARGS 16

/* ---- simple readline ---- */
static int readline(char *buf, int maxlen) {
    int pos = 0;
    buf[0] = 0;
    while (1) {
        char c = keyboard_getchar();
        if (c == '\n' || c == '\r') {
            print_char('\n');
            buf[pos] = 0;
            return pos;
        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                buf[pos] = 0;
                print_char('\b');
            }
        } else if (pos < maxlen - 1) {
            buf[pos++] = c;
            buf[pos]   = 0;
            print_char(c);
        }
    }
}

/* ---- arg parser ---- */
static int parse_args(char *cmd, char **argv) {
    int argc = 0;
    char *p = cmd;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
        if (*p) { *p = 0; p++; }
        if (argc >= MAX_ARGS) break;
    }
    return argc;
}

/* ---- built-in commands ---- */
static void cmd_help() {
    set_color(MAKE_COLOR(COLOR_LCYAN, COLOR_BLACK));
    print("HexShell v1.0 — Comandi disponibili:\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("  help          mostra questo messaggio\n");
    print("  version       versione del sistema\n");
    print("  clear         pulisce lo schermo\n");
    print("  echo [testo]  stampa testo\n");
    print("  mem           info memoria\n");
    print("  color [n]     cambia colore testo (0-15)\n");
    print("  reboot        riavvia il sistema\n");
    print("  shutdown      spegne il sistema\n");
    print("  uname         info kernel\n");
    print("  neofetch      info sistema\n");
    print("  hexlife       ?\n");
}

static void cmd_version() {
    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("HexOS_! v0.2.0 — Built from scratch.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
}

static void cmd_uname() {
    print("HexOS_! 0.2.0 hexos i686 GNU/HexOS\n");
}

static void cmd_mem() {
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("Heap base: 0x01000000\n");
    print("Allocazioni attive: OK\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
}

static void cmd_neofetch() {
    set_color(MAKE_COLOR(COLOR_LBLUE, COLOR_BLACK));
    print("        .HHHHHHH.\n");
    print("       HHHHHHHHHHH\n");
    print("      HHH       HHH\n");
    print("     HHH         HHH\n");
    print("      HHH       HHH\n");
    print("       HHHHHHHHHHH\n");
    print("        'HHHHHHH'\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("\n");
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK)); print("OS     ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK)); print(": HexOS_! v0.2.0 i686\n");
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK)); print("Kernel ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK)); print(": hexos-0.2.0-bare\n");
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK)); print("Shell  ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK)); print(": HexShell v1.0\n");
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK)); print("CPU    ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK)); print(": i686 (Protected Mode)\n");
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK)); print("Boot   ");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK)); print(": GRUB Multiboot\n");
}

static void cmd_color(char **argv, int argc) {
    if (argc < 2) { print("uso: color [0-15]\n"); return; }
    int n = atoi(argv[1]);
    if (n < 0 || n > 15) { print("colore non valido (0-15)\n"); return; }
    set_color(MAKE_COLOR(n, COLOR_BLACK));
    print("Colore impostato.\n");
}

static void cmd_echo(char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        print(argv[i]);
        if (i < argc-1) print_char(' ');
    }
    print_char('\n');
}

static void cmd_reboot() {
    print("Riavvio...\n");
    /* pulse keyboard controller reset line */
    unsigned char tmp;
    __asm__ volatile ("cli");
    /* flush keyboard buffer */
    do { tmp = 0; __asm__ volatile ("inb $0x64, %0":"=a"(tmp)); } while(tmp & 2);
    __asm__ volatile ("outb %0, $0x64" : : "a"((unsigned char)0xFE));
    __asm__ volatile ("hlt");
}

static void cmd_shutdown() {
    print("Arresto in corso...\n");
    /* ACPI S5 / QEMU power-off via port 0x604 */
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x2000), "Nd"((unsigned short)0x604));
    /* fallback: Bochs/older QEMU */
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x0 ), "Nd"((unsigned short)0xB004));
    print("Puoi spegnere la macchina.\n");
    __asm__ volatile ("cli; hlt");
}

static void cmd_hexlife() {
    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("42. Ovviamente.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
}

/* ---- main shell loop ---- */
void shell_start() {
    char cmd[CMD_LEN];
    char *argv[MAX_ARGS];
    int argc;

    set_color(MAKE_COLOR(COLOR_LCYAN, COLOR_BLACK));
    print("\nHexShell v1.0 — digita 'help' per i comandi\n\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));

    while (1) {
        set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
        print("hex@hexos");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        print(":~$ ");

        readline(cmd, CMD_LEN);

        argc = parse_args(cmd, argv);
        if (argc == 0) continue;

        if      (strcmp(argv[0], "help")     == 0) cmd_help();
        else if (strcmp(argv[0], "version")  == 0) cmd_version();
        else if (strcmp(argv[0], "clear")    == 0) clear_screen();
        else if (strcmp(argv[0], "echo")     == 0) cmd_echo(argv, argc);
        else if (strcmp(argv[0], "mem")      == 0) cmd_mem();
        else if (strcmp(argv[0], "color")    == 0) cmd_color(argv, argc);
        else if (strcmp(argv[0], "reboot")   == 0) cmd_reboot();
        else if (strcmp(argv[0], "shutdown") == 0) cmd_shutdown();
        else if (strcmp(argv[0], "uname")    == 0) cmd_uname();
        else if (strcmp(argv[0], "neofetch") == 0) cmd_neofetch();
        else if (strcmp(argv[0], "hexlife")  == 0) cmd_hexlife();
        else {
            set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
            print("hexsh: ");
            print(argv[0]);
            print(": comando non trovato\n");
            set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        }
    }
}

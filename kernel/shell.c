#include "screen.h"
#include "string.h"
#include "keyboard.h"
#include "shell.h"
#include "memory.h"
#include "auth.h"
#include "security.h"

#define CMD_LEN     128
#define MAX_ARGS    16
#define HISTORY_LEN 16

/* Guard value placed right after the command buffer. If a bug or a
 * deliberately crafted command ever wrote past CMD_LEN, this value
 * would no longer match and the shell would notice instead of
 * silently running with corrupted memory. It is a small local stand
 * in for a real stack protector, which the toolchain does not give
 * us here since the kernel is built with -fno-stack-protector. */
#define CANARY_VALUE 0xC0FFEEu

/* ---- command history ---- */
static char history[HISTORY_LEN][CMD_LEN];
static int history_count = 0;

static void history_add(const char *cmd) {
    if (cmd[0] == 0) return;
    strcpy(history[history_count % HISTORY_LEN], cmd);
    history_count++;
}

static void cmd_history(void) {
    int start = 0;
    int total = history_count;

    if (total == 0) {
        print("History is empty.\n");
        return;
    }
    if (total > HISTORY_LEN) {
        start = history_count % HISTORY_LEN;
        total = HISTORY_LEN;
    }
    for (int i = 0; i < total; i++) {
        int index = (start + i) % HISTORY_LEN;
        print_int(i + 1);
        print("  ");
        print(history[index]);
        print_char('\n');
    }
}

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
    print("HexShell v1.0, available commands:\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    print("  help          show this message\n");
    print("  version       show system version\n");
    print("  clear         clear the screen\n");
    print("  echo [text]   print text\n");
    print("  mem           show memory info\n");
    print("  color [n]     change text color (0-15)\n");
    print("  whoami        show the current user\n");
    print("  passwd        change the account password\n");
    print("  lock          lock the session\n");
    print("  history       show command history\n");
    print("  audit         show the security audit log\n");
    print("  checksum [s]  compute a checksum of the given text\n");
    print("  integrity     verify kernel data integrity\n");
    print("  reboot        restart the system\n");
    print("  shutdown      power off the system\n");
    print("  uname         show kernel info\n");
    print("  neofetch      show system info\n");
    print("  hexlife       the answer to everything\n");
}

static void cmd_version() {
    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("HexOS_! v0.2.0, built from scratch.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
}

static void cmd_uname() {
    print("HexOS_! 0.2.0 hexos i686 GNU/HexOS\n");
}

static void cmd_mem() {
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("Heap base: 0x01000000\n");
    print("Active allocations: OK\n");
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
    if (argc < 2) { print("usage: color [0-15]\n"); return; }
    int n = atoi(argv[1]);
    if (n < 0 || n > 15) { print("invalid color, use a value from 0 to 15\n"); return; }
    set_color(MAKE_COLOR(n, COLOR_BLACK));
    print("Color set.\n");
}

static void cmd_echo(char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        print(argv[i]);
        if (i < argc-1) print_char(' ');
    }
    print_char('\n');
}

static void cmd_whoami() {
    print(auth_current_user());
    print_char('\n');
}

static void cmd_passwd() {
    char new_pass[AUTH_PASS_LEN];
    char confirm[AUTH_PASS_LEN];

    print("New password: ");
    auth_read_secret(new_pass, AUTH_PASS_LEN);
    print("Confirm password: ");
    auth_read_secret(confirm, AUTH_PASS_LEN);

    if (strcmp(new_pass, confirm) != 0) {
        set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
        print("Passwords do not match, nothing was changed.\n");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        return;
    }
    if (new_pass[0] == 0) {
        set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
        print("Password cannot be empty, nothing was changed.\n");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        return;
    }

    auth_set_password(new_pass);
    set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
    print("Password updated. It will reset on the next reboot since\n");
    print("there is no disk storage yet.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
}

static void cmd_checksum(char **argv, int argc) {
    if (argc < 2) { print("usage: checksum [text]\n"); return; }

    char joined[CMD_LEN];
    joined[0] = 0;
    for (int i = 1; i < argc; i++) {
        strcat(joined, argv[i]);
        if (i < argc - 1) strcat(joined, " ");
    }

    unsigned int sum = security_checksum(joined);
    print("Checksum: ");
    print_hex(sum);
    print_char('\n');
}

static void cmd_reboot() {
    print("Restarting...\n");
    security_log("Reboot requested");
    /* pulse keyboard controller reset line */
    unsigned char tmp;
    __asm__ volatile ("cli");
    /* flush keyboard buffer */
    do { tmp = 0; __asm__ volatile ("inb $0x64, %0":"=a"(tmp)); } while(tmp & 2);
    __asm__ volatile ("outb %0, $0x64" : : "a"((unsigned char)0xFE));
    __asm__ volatile ("hlt");
}

static void cmd_shutdown() {
    print("Shutting down...\n");
    security_log("Shutdown requested");
    /* ACPI S5 / QEMU power-off via port 0x604 */
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x2000), "Nd"((unsigned short)0x604));
    /* fallback: Bochs/older QEMU */
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x0 ), "Nd"((unsigned short)0xB004));
    print("It is now safe to power off the machine.\n");
    __asm__ volatile ("cli; hlt");
}

static void cmd_hexlife() {
    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("42. Obviously.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
}

/* ---- main shell loop ---- */
void shell_start() {
    char cmd[CMD_LEN];
    volatile unsigned int canary = CANARY_VALUE;
    char *argv[MAX_ARGS];
    int argc;

    set_color(MAKE_COLOR(COLOR_LCYAN, COLOR_BLACK));
    print("\nHexShell v1.0, type 'help' for a list of commands\n\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));

    while (1) {
        set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
        print("hex@hexos");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        print(":~$ ");

        readline(cmd, CMD_LEN);

        if (canary != CANARY_VALUE) {
            set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
            print("\n*** STACK CANARY CORRUPTED, HALTING ***\n");
            security_log("Stack canary corruption detected");
            __asm__ volatile ("cli");
            for (;;) { __asm__ volatile ("hlt"); }
        }

        argc = parse_args(cmd, argv);
        if (argc == 0) continue;

        history_add(cmd);

        if      (strcmp(argv[0], "help")      == 0) cmd_help();
        else if (strcmp(argv[0], "version")   == 0) cmd_version();
        else if (strcmp(argv[0], "clear")     == 0) clear_screen();
        else if (strcmp(argv[0], "echo")      == 0) cmd_echo(argv, argc);
        else if (strcmp(argv[0], "mem")       == 0) cmd_mem();
        else if (strcmp(argv[0], "color")     == 0) cmd_color(argv, argc);
        else if (strcmp(argv[0], "whoami")    == 0) cmd_whoami();
        else if (strcmp(argv[0], "passwd")    == 0) cmd_passwd();
        else if (strcmp(argv[0], "lock")      == 0) auth_lock_session();
        else if (strcmp(argv[0], "history")   == 0) cmd_history();
        else if (strcmp(argv[0], "audit")     == 0) security_print_log();
        else if (strcmp(argv[0], "checksum")  == 0) cmd_checksum(argv, argc);
        else if (strcmp(argv[0], "integrity") == 0) security_check_integrity();
        else if (strcmp(argv[0], "reboot")    == 0) cmd_reboot();
        else if (strcmp(argv[0], "shutdown")  == 0) cmd_shutdown();
        else if (strcmp(argv[0], "uname")     == 0) cmd_uname();
        else if (strcmp(argv[0], "neofetch")  == 0) cmd_neofetch();
        else if (strcmp(argv[0], "hexlife")   == 0) cmd_hexlife();
        else {
            set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
            print("hexsh: ");
            print(argv[0]);
            print(": command not found\n");
            set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        }
    }
}

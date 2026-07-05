#include "auth.h"
#include "screen.h"
#include "string.h"
#include "keyboard.h"
#include "security.h"

#define MAX_LOGIN_ATTEMPTS 3

static const char username[AUTH_USER_LEN] = "root";
/* Default credential for a fresh install. Change it with "passwd"
 * on first boot, it is stored in plain memory and there is no disk
 * persistence yet, so it resets to this value on every reboot. */
static char stored_password[AUTH_PASS_LEN] = "hexos";

int auth_read_secret(char *buf, int maxlen) {
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
            buf[pos] = 0;
            print_char('*');
        }
    }
}

const char *auth_current_user(void) {
    return username;
}

void auth_set_password(const char *new_pass) {
    int len = strlen(new_pass);
    if (len >= AUTH_PASS_LEN) len = AUTH_PASS_LEN - 1;
    for (int i = 0; i < len; i++) stored_password[i] = new_pass[i];
    stored_password[len] = 0;
    security_log("Password changed");
}

void auth_login(void) {
    char input[AUTH_PASS_LEN];
    int attempts = 0;

    while (attempts < MAX_LOGIN_ATTEMPTS) {
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        print("hexos login: ");
        print(username);
        print_char('\n');
        print("password: ");

        auth_read_secret(input, AUTH_PASS_LEN);

        if (strcmp(input, stored_password) == 0) {
            set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
            print("Login successful.\n");
            set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
            security_log("Login successful");
            return;
        }

        attempts++;
        set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
        print("Incorrect password.\n");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        security_log("Failed login attempt");
    }

    set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
    print("\nToo many failed attempts. System halted.\n");
    print("Restart the machine to try again.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    security_log("Account locked after repeated failures");

    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void auth_lock_session(void) {
    char input[AUTH_PASS_LEN];

    set_color(MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print("\nSession locked. Enter password to continue.\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
    security_log("Session locked");

    while (1) {
        print("password: ");
        auth_read_secret(input, AUTH_PASS_LEN);

        if (strcmp(input, stored_password) == 0) {
            set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
            print("Session unlocked.\n");
            set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
            security_log("Session unlocked");
            return;
        }

        set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
        print("Incorrect password.\n");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        security_log("Failed unlock attempt");
    }
}

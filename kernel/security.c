#include "security.h"
#include "screen.h"
#include "string.h"

static char audit_log[AUDIT_LOG_SIZE][AUDIT_MSG_LEN];
static int  audit_count = 0;
static unsigned int event_tick = 0;

/* Reference checksum computed once at boot over a fixed piece of
 * kernel data. If it ever changes at runtime, something modified
 * memory it should not have. This is a tamper-evidence check, not
 * a guarantee, since the checksum itself lives in writable RAM. */
static const char boot_reference[] = "HexOS_! 0.2.0 trusted boot reference";
static unsigned int baseline_checksum = 0;

unsigned int security_checksum(const char *str) {
    /* FNV-1a, 32 bit. Fast and good enough to catch accidental or
     * casual tampering. Do not rely on this for anything that needs
     * real cryptographic guarantees. */
    unsigned int hash = 0x811C9DC5u;
    while (*str) {
        hash ^= (unsigned char)(*str++);
        hash *= 0x01000193u;
    }
    return hash;
}

void security_baseline_init(void) {
    baseline_checksum = security_checksum(boot_reference);
    audit_count = 0;
    event_tick = 0;
    security_log("Baseline integrity checksum recorded");
}

void security_check_integrity(void) {
    unsigned int current = security_checksum(boot_reference);
    if (current == baseline_checksum) {
        set_color(MAKE_COLOR(COLOR_LGREEN, COLOR_BLACK));
        print("Integrity check passed. Reference checksum matches baseline.\n");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        security_log("Integrity check passed");
    } else {
        set_color(MAKE_COLOR(COLOR_LRED, COLOR_BLACK));
        print("Integrity check FAILED. Reference data does not match baseline.\n");
        set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));
        security_log("Integrity check FAILED");
    }
}

void security_log(const char *event) {
    char *slot = audit_log[audit_count % AUDIT_LOG_SIZE];
    char tick_str[12];

    itoa((int)event_tick, tick_str);

    slot[0] = 0;
    strcat(slot, "[");
    strcat(slot, tick_str);
    strcat(slot, "] ");
    strcat(slot, event);

    audit_count++;
    event_tick++;
}

void security_print_log(void) {
    int start = 0;
    int total = audit_count;

    if (total == 0) {
        print("Audit log is empty.\n");
        return;
    }

    if (total > AUDIT_LOG_SIZE) {
        start = audit_count % AUDIT_LOG_SIZE;
        total = AUDIT_LOG_SIZE;
    }

    set_color(MAKE_COLOR(COLOR_LCYAN, COLOR_BLACK));
    print("Security audit log (event id in brackets):\n");
    set_color(MAKE_COLOR(COLOR_BWHITE, COLOR_BLACK));

    for (int i = 0; i < total; i++) {
        int index = (start + i) % AUDIT_LOG_SIZE;
        print("  ");
        print(audit_log[index]);
        print_char('\n');
    }
}

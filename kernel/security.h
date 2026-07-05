#ifndef SECURITY_H
#define SECURITY_H

/* Lightweight security helpers for HexOS.
 *
 * None of this is meant to compete with a real TPM or a hardened
 * crypto stack, there is no hardware RNG and no timer driver yet,
 * so "checksum" here means a simple non-cryptographic hash (FNV-1a)
 * and the audit log uses an internal event counter instead of a
 * wall clock timestamp. It is still useful for tamper evidence and
 * for keeping a record of what happened during a session. */

#define AUDIT_LOG_SIZE 32
#define AUDIT_MSG_LEN  80

void security_log(const char *event);
void security_print_log(void);
unsigned int security_checksum(const char *str);
void security_baseline_init(void);
void security_check_integrity(void);

#endif

#ifndef AUTH_H
#define AUTH_H

#define AUTH_USER_LEN 32
#define AUTH_PASS_LEN 32

/* Runs the login prompt at boot. Halts the machine after too many
 * failed attempts. Returns once the correct password is entered. */
void auth_login(void);

/* Locks the current session and blocks until the correct password
 * is entered again. Used by the "lock" shell command. */
void auth_lock_session(void);

/* Reads a line of input without echoing the real characters, an
 * asterisk is printed for each character instead. */
int auth_read_secret(char *buf, int maxlen);

/* Replaces the stored password with a new one, bounded to
 * AUTH_PASS_LEN - 1 characters. */
void auth_set_password(const char *new_pass);

const char *auth_current_user(void);

#endif

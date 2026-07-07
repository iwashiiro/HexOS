#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init();
char keyboard_getchar();   /* blocking: wait for a keypress */
void keyboard_handler();   /* called by ISR */

#endif

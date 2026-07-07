#include "string.h"

int strlen(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return *(unsigned char*)a - *(unsigned char*)b;
}

int strncmp(const char *a, const char *b, int n) {
    while (n-- && *a && (*a == *b)) { a++; b++; }
    if (n < 0) return 0;
    return *(unsigned char*)a - *(unsigned char*)b;
}

void strcpy(char *dest, const char *src) {
    while (*src) *dest++ = *src++;
    *dest = 0;
}

void strcat(char *dest, const char *src) {
    while (*dest) dest++;
    while (*src) *dest++ = *src++;
    *dest = 0;
}

char *strchr(const char *str, char c) {
    while (*str) {
        if (*str == c) return (char*)str;
        str++;
    }
    return 0;
}

void memset(void *ptr, int val, int size) {
    unsigned char *p = (unsigned char*)ptr;
    while (size--) *p++ = (unsigned char)val;
}

void memcpy(void *dest, const void *src, int size) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    while (size--) *d++ = *s++;
}

int atoi(const char *s) {
    int n = 0, neg = 0;
    if (*s == '-') { neg = 1; s++; }
    while (*s >= '0' && *s <= '9') n = n * 10 + (*s++ - '0');
    return neg ? -n : n;
}

void itoa(int n, char *buf) {
    char tmp[12]; int i = 0, neg = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) { buf[0]='0'; buf[1]=0; return; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    if (neg) tmp[i++] = '-';
    for (int j = 0; j < i; j++) buf[j] = tmp[i-1-j];
    buf[i] = 0;
}

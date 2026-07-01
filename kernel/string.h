#ifndef STRING_H
#define STRING_H

int    strlen(const char *str);
int    strcmp(const char *a, const char *b);
int    strncmp(const char *a, const char *b, int n);
void   strcpy(char *dest, const char *src);
void   strcat(char *dest, const char *src);
char  *strchr(const char *str, char c);
void   memset(void *ptr, int val, int size);
void   memcpy(void *dest, const void *src, int size);
int    atoi(const char *s);
void   itoa(int n, char *buf);

#endif

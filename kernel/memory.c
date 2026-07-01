#include "memory.h"

static unsigned int heap = 0x1000000;

void memory_init()
{
    heap = 0x1000000;
}

void *kmalloc(int size)
{
    void *ptr = (void*)heap;
    heap += size;
    return ptr;
}
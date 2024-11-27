#include "heap.hpp"

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);
extern "C" void free_(void *p);

uint32_t kmalloc(uint32_t size)
{
    return kmalloc_(size, 0, 0);
}

void kfree(void *p)
{
    free_(p);
}
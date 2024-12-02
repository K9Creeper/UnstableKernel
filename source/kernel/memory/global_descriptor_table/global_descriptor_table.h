#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define KERNEL_MEMORY_GDT_ENTRYCOUNT 5

#ifdef __cplusplus

extern "C" void Kernel_Memory_GDT_Init();
extern "C" void Kernel_Memory_GDT_Install();

#else

extern void Kernel_Memory_GDT_Init();
extern void Kernel_Memory_GDT_Install();

#endif

#endif
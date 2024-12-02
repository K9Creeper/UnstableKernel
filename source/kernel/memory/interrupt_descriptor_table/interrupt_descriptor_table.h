#ifndef IDT_H
#define IDT_H

#ifdef __cplusplus

extern "C" void Kernel_Memory_IDT_Init();
extern "C" void Kernel_Memory_IDT_Install();

#else

extern void Kernel_Memory_IDT_Init();
extern void Kernel_Memory_IDT_Install();

#endif

#endif

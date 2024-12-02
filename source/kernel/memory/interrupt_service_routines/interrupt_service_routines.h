#ifndef ISRS_H
#define ISRS_H

#include <stdint.h>

struct Kernel_Memory_ISR_Regs
{
    unsigned int gs, fs, es, ds;                         /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
    unsigned int int_no, err_code;                       /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;           /* pushed by the processor automatically */
};

#ifdef __cplusplus

extern "C" void Kernel_Memory_ISR_Install();

#else

extern void Kernel_Memory_ISR_Install();

#endif


#endif
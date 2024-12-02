#ifndef IRQ_H
#define IRQ_H

#include "../interrupt_service_routines/interrupt_service_routines.h"

typedef void (*Kernel_Memory_IRQ_fHandle)(struct Kernel_Memory_ISR_Regs *r);

#ifdef __cplusplus

extern "C" void Kernel_Memory_IRQ_Install();

extern "C" void Kernel_Memory_IRQ_InstallHandler(int irq, void* handler);

extern "C" void Kernel_Memory_IRQ_UninstallHandler(int irq);

#else

extern void Kernel_Memory_IRQ_Install();

extern void Kernel_Memory_IRQ_InstallHandler(int irq, void* handler);

extern void Kernel_Memory_IRQ_UninstallHandler(int irq);


#endif

#endif




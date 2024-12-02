#ifndef IRQ_H
#define IRQ_H

#include "../interrupt_service/interrupt_service.h"

#include <stdint.h>

typedef void(*Kernel_Memory_IRQ_Handle)(struct Registers regs);

#ifdef __cplusplus

extern "C" void Kernel_Memory_IRQ_AddHandle(int num, void* handle);
extern "C" void Kernel_Memory_IRQ_RemoveHandle(int num);

#else

extern void Kernel_Memory_IRQ_AddHandle(int num, void* handle);
extern void Kernel_Memory_IRQ_RemoveHandle(int num);

#endif

#endif
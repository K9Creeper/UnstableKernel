#include "interrupt_descriptor_table.h"

#include "../../../c_helpers/memory.h"

#include <stdint.h>

struct IDTEntry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;     
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct IDTPtr
{
    unsigned short limit; // Size of the IDT in bytes
    unsigned int base;    // Base address of the IDT
} __attribute__((packed));

struct IDTEntry pIDT[256];
struct IDTPtr _pIDT;

extern void _idt_load();

void IDTSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    pIDT[num].base_hi = (base >> 16) & 0xFFFF;
    pIDT[num].base_lo = (base & 0xFFFF);
    pIDT[num].always0 = 0;
    pIDT[num].sel = sel;
    pIDT[num].flags = flags;
}

extern void printf(const char *format, ...);

void Kernel_Memory_IDT_Init()
{
    _pIDT.limit = (sizeof(struct IDTEntry) * 256) - 1;
    _pIDT.base = (unsigned int)(&pIDT);

    printf("IDT limit: %d. IDT base: %x\n", _pIDT.limit, _pIDT.base);

    memset((unsigned char *)(&pIDT), 0, sizeof(struct IDTEntry) * 256);
}

void Kernel_Memory_IDT_Install()
{
    _idt_load();
}
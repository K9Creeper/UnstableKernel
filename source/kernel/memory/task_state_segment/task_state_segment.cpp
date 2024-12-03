#include "task_state_segment.hpp"

#include "../global_descriptor_table/global_descriptor_table.hpp"

#include "../../../c_helpers/memory.h"

struct TSS
{
    uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
    uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
    uint32_t ss0;      // The stack segment to load when changing to kernel mode.
                       // Everything below here is unused.
    uint32_t esp1;     // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

TSS _tss_entry;

extern "C" void _flush_tss();
extern "C" uint32_t _get_eip();
extern "C" void _gdt_flush();

void zero_tss(uint32_t limit)
{
    memset((unsigned char *)&_tss_entry, 0, limit);
}

extern void GDTSetGate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

void tss_write(int index, uint16_t ss0, uint32_t esp0)
{
    /* https://wiki.osdev.org/Getting_to_Ring_3 */
    uint32_t eip = _get_eip();
    uint32_t base = (uint32_t)&_tss_entry;
    uint32_t limit = base + sizeof(_tss_entry);

    GDTSetGate(index, (uint32_t)&_tss_entry, limit, 0x89, 0x0);

    zero_tss(sizeof(_tss_entry));

    _tss_entry.ss0 = ss0;
    _tss_entry.esp0 = esp0;
    _tss_entry.eip = eip;
    _tss_entry.cs = 0x0b;
    _tss_entry.ss = 0x13;
    _tss_entry.ds = 0x13;
    _tss_entry.es = 0x13;
    _tss_entry.fs = 0x13;
    _tss_entry.gs = 0x13;
    _tss_entry.iomap_base = sizeof(TSS);
}

void tss_set_kernel_stack(uint32_t stack)
{
    // Used when an interrupt occurs
    _tss_entry.esp0 = stack;
}

void Kernel::Memory::TSS::Install()
{
    tss_write(5, 0x10, 0x00100000);

    _flush_tss();

    _gdt_flush();
}
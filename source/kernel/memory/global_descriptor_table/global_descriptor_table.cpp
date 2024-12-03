#include "global_descriptor_table.hpp"

struct GDTEntry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct GDTPtr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

GDTEntry pGDT[KERNEL_MEMORY_GDT_ENTRYCOUNT];

extern "C" void _gdt_flush();

GDTPtr _pGDT;

void GDTSetGate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    pGDT[index].base_low = (base & 0xFFFF);
    pGDT[index].base_middle = (base >> 16) & 0xFF;
    pGDT[index].base_high = (base >> 24) & 0xFF;

    pGDT[index].limit_low = (limit & 0xFFFF);
    pGDT[index].granularity = ((limit >> 16) & 0x0F);

    pGDT[index].granularity |= (gran & 0xF0);
    pGDT[index].access = access;
}

void Kernel::Memory::GDT::Init()
{
    _pGDT.limit = (sizeof(struct GDTEntry) * KERNEL_MEMORY_GDT_ENTRYCOUNT) - 1;
    _pGDT.base = (unsigned int)(&pGDT);
}

void Kernel::Memory::GDT::Install()
{
    /* Set Gates */
    {
        GDTSetGate(0, 0, 0, 0, 0);
    }

    {
        /* Code Segment */
        GDTSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

        /* Data Segment. Same as the code segment, but different access byte */
        GDTSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    }

    {
        /* Code Segment */
        GDTSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 
        /* Data Segment */
        GDTSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    }

    /* Flush */
    _gdt_flush();
}
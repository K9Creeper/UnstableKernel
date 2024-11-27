#include "global_descriptor_table.hpp"

namespace Kernel
{
    namespace Memory
    {
        namespace GDT
        {
            bool bInitialized = false;

            GDTEntry pGDT[KERNEL_MEMORY_GDT_ENTRYCOUNT];
        }
    }
}

extern "C" void _gdt_flush();

Kernel::Memory::GDT::GDTPtr _pGDT;

void GDTSetGate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    Kernel::Memory::GDT::GDTEntry *entry = &Kernel::Memory::GDT::pGDT[index];

    entry->segment_limit = limit & 0xFFFF;
    entry->base_low = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->access = access;

    entry->granularity = (limit >> 16) & 0x0F;
    entry->granularity = entry->granularity | (gran & 0xF0);

    entry->base_high = (base >> 24 & 0xFF);
}

void Kernel::Memory::GDT::Init()
{
    if (Kernel::Memory::GDT::bInitialized)
        return;

    Kernel::Memory::GDT::bInitialized = true;

    _pGDT.limit = (sizeof(Kernel::Memory::GDT::GDTEntry) * KERNEL_MEMORY_GDT_ENTRYCOUNT) - 1;
    _pGDT.base = reinterpret_cast<unsigned int>(&pGDT);
}

void Kernel::Memory::GDT::Install()
{
    if (!Kernel::Memory::GDT::bInitialized)
        return;

    /* Set Gates */

    {
        /* Code Segment */
        GDTSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

        /* Data Segment. Same as the code segment, but different access byte */
        GDTSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    }

    {
        // USER code segment
        GDTSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

        // USER data segment
        GDTSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    }

    /* Flush */
    _gdt_flush();
}
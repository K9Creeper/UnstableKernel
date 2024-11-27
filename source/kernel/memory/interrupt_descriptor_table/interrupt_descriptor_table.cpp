#include "interrupt_descriptor_table.hpp"

#include "../../../c_helpers/memory.h"

namespace Kernel
{
    namespace Memory
    {
        namespace IDT
        {
            bool bInitialized = false;

            IDTEntry pIDT[256];
        }
    }
}

extern "C" void _idt_load();

Kernel::Memory::IDT::IDTPtr _pIDT;

void IDTSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags){
    Kernel::Memory::IDT::pIDT[num].base_hi = (base >> 16) & 0xFFFF;
    Kernel::Memory::IDT::pIDT[num].base_lo = (base & 0xFFFF);
    Kernel::Memory::IDT::pIDT[num].ZERO = 0;
    Kernel::Memory::IDT::pIDT[num].sel = sel;
    Kernel::Memory::IDT::pIDT[num].flags = flags;
}

void Kernel::Memory::IDT::Init(){
    if(Kernel::Memory::IDT::bInitialized)
        return;

    Kernel::Memory::IDT::bInitialized = true;

    _pIDT.limit = (sizeof (Kernel::Memory::IDT::IDTEntry) * 256) - 1;
    _pIDT.base = reinterpret_cast<unsigned int>(&Kernel::Memory::IDT::pIDT);
}
            
void Kernel::Memory::IDT::Install(){
    if(!Kernel::Memory::IDT::bInitialized)
        return;

    /* clear entire IDT */
    memset(reinterpret_cast<unsigned char*>(&Kernel::Memory::IDT::pIDT), 0, sizeof(Kernel::Memory::IDT::IDTEntry) * 256);

    /* Add new ISRs  */
    _idt_load();
}
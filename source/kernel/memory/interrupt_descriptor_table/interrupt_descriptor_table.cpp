/// ------------------------------
/// interrupt_descriptor_table.hpp
/// @brief This file defines core functions to setup the Interrupt Descriptor Table, 
/// aswell as setting up all interrupt gates.

#include "interrupt_descriptor_table.hpp"

#include "../../../chelpers/memory.h"

#include "../interrupt_request/interrupt_request.hpp"
#include "../interrupt_service/interrupt_service.hpp"

#include <stdint.h>

Kernel::Memory::IDT::IDTEntry pIDT[256];
Kernel::Memory::IDT::IDTPtr _pIDT;

extern "C" void _idt_load();

void IDTSetGate(unsigned char num, uint32_t base, unsigned short sel, unsigned char flags)
{
    pIDT[num].base_hi = (base >> 16) & 0xFFFF;
    pIDT[num].base_lo = (base & 0xFFFF);
    pIDT[num].always0 = 0;
    pIDT[num].sel = sel;
    pIDT[num].flags = flags;
}

void Kernel::Memory::IDT::Init()
{
    _pIDT.limit = (sizeof(IDTEntry) * 256) - 1;
    _pIDT.base = (unsigned int)(&pIDT);
    memset((unsigned char *)(&pIDT), 0, sizeof(IDTEntry) * 256);
}

namespace Kernel{
    namespace Memory{
        namespace IRQ{
            extern void Remap();
            extern void AddGates();
        }

        namespace ISRS{
            extern void AddGates();
        }
    }
}

void Kernel::Memory::IDT::Install()
{
    Kernel::Memory::IRQ::Remap();

    Kernel::Memory::ISRS::AddGates();

    Kernel::Memory::IRQ::AddGates();

    _idt_load();
}
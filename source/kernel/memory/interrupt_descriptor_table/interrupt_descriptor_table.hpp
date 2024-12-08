/// ------------------------------
/// interrupt_descriptor_table.hpp
/// @brief This file declares the needed function to setup the Global Descriptor Table.

#pragma once

namespace Kernel
{
    namespace Memory
    {
        namespace IDT
        {
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
                unsigned short limit; 
                unsigned int base;    
            } __attribute__((packed));

            extern void Init();
            extern void Install();
        }
    }
}

extern Kernel::Memory::IDT::IDTEntry pIDT[256];
extern Kernel::Memory::IDT::IDTPtr _pIDT;
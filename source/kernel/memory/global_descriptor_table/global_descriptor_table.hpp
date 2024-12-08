/// ---------------------------
/// global_descriptor_table.hpp
/// @brief This file declares the needed function to setup the Global Descriptor Table.

#pragma once

#include <stdint.h>

#define KERNEL_MEMORY_GDT_ENTRYCOUNT 8

namespace Kernel
{
    namespace Memory
    {
        namespace GDT
        {
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

            extern void Init();
            extern void Install();
        }
    }
}

extern Kernel::Memory::GDT::GDTEntry pGDT[KERNEL_MEMORY_GDT_ENTRYCOUNT];
extern Kernel::Memory::GDT::GDTPtr _pGDT;
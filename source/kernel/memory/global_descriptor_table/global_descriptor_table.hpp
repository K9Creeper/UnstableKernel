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
                unsigned short segment_limit; // segment limit first 0-15 bits
                unsigned short base_low;      // base first 0-15 bits
                unsigned char base_middle;    // base 16-23 bits
                unsigned char access;         // access byte
                unsigned char granularity;    // high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
                unsigned char base_high;      // base 24-31 bits
            } __attribute__((packed));

            struct GDTPtr
            {
                unsigned short limit;
                unsigned int base;
            } __attribute__((packed));

            extern bool bInitialized;

            extern GDTEntry pGDT[KERNEL_MEMORY_GDT_ENTRYCOUNT];

            void Init();

            void Install();
        }
    }
}
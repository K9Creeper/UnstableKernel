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
                unsigned short sel; /* kernel segment */
                unsigned char ZERO; /* ALWAYS 0 */
                unsigned char flags;
                unsigned short base_hi;
            } __attribute__((packed));

            struct IDTPtr
            {
                unsigned short limit;
                unsigned int base;
            } __attribute__((packed));

            extern bool bInitialized;

            extern IDTEntry pIDT[256];

            void Init();

            void Install();
        }
    }
}
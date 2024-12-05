#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace KHeap
        {
            extern bool bInitialized;

            struct Block
            {
                unsigned int size;
                struct Block *prev;
                struct Block *next;
            };

            namespace Early
            {
                extern uint32_t nextPlacementAddress;
                extern void PreInit(uint32_t kernel_end);

                extern uint32_t pkmalloc_(uint32_t size, bool shouldAlign = false, uint32_t *physAddress = nullptr);

            }

            extern void Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly);
        }
    }
}
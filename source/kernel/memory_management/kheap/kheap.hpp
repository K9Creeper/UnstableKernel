#pragma once

#include <stdint.h>

#include "kheap_ordered_array.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace KHeap
        {
            extern bool bInitialized;
            
            struct Heap
            {
                KHeapOrderedArray index;

                bool supervisor;
                bool readonly;
            };

            struct Header
            {
                uint32_t magic; // for error checking and identification
                bool is_hole;
                uint32_t size; // Size of block, including header(this) and footer
            };

            struct Footer
            {
                uint32_t magic;
                Header* header;
            };

            namespace Early
            {
                extern uint32_t nextPlacementAddress;
                extern void PreInit(uint32_t kernel_end);

                extern uint32_t pkmalloc_(uint32_t size, bool shouldAlign = false, uint32_t *physAddress = nullptr);
            }

            extern void Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor = false, bool readonly = false);

            extern uint32_t kmalloc_(uint32_t size, bool shouldAlign = false, uint32_t* physAddress = nullptr);
        }
    }
}
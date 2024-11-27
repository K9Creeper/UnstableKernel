#pragma once

#include <stdint.h>

#include "../../../c_helpers/ordered_array.h"

namespace Kernel
{
    namespace Memory
    {
        namespace KHeap
        {

            struct Heap
            {
                ordered_array_t index;

                uint32_t startAddress;
                uint32_t endAddress;
                uint32_t maxAddress;
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

            static Heap *heap = nullptr;

            extern bool bInitialized;

            extern uint32_t placementAddress;

            extern void Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly);

        }
    }
}
#pragma once

#include <stdint.h>

#include "../../../cpphelpers/ordered_array.hpp"

namespace Kernel
{
    namespace Memory
    {
        namespace KHeap
        {
            extern bool bInitialized;

            struct Header
            {
                uint32_t magic; // for error checking and identification
                uint8_t is_hole;
                uint32_t size; // Size of block, including header(this) and footer
            };

            struct Footer
            {
                uint32_t magic;
                Header *header;
            };

            namespace Early
            {
                extern uint32_t nextPlacementAddress;
                extern void PreInit(uint32_t kernel_end);

                extern uint32_t pkmalloc_(uint32_t size, bool shouldAlign = false, uint32_t *physAddress = nullptr);

            }

            extern uint32_t kmalloc_(uint32_t size, bool shouldAlign = false, uint32_t *physAddress = nullptr);
            extern void Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly);
        }
    }
}

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);
extern "C" void kfree_(void* ptr);
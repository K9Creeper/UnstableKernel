#pragma once

#include <stdint.h>

#include "../../../c_helpers/ordered_array.h"

#define KHEAP_START         0xC0000000  // arbitrary
#define KHEAP_INITIAL_SIZE  0x100000    // arbitrary

#define HEAP_INDEX_SIZE     0x20000     // arbitrary
#define HEAP_MAGIC          0xDEADBEEF  // unusual number that will stand out from others
#define HEAP_MIN_SIZE       0x70000     // arbitrary

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
                uint8_t supervisor;
                uint8_t readonly;
            };

            struct Header
            {
                uint32_t magic; // for error checking and identification
                uint8_t is_hole;
                uint32_t size; // Size of block, including header(this) and footer
            };

            struct Footer
            {
                uint32_t magic;
                Header* header;
            };

            extern Heap *heap;

            extern bool bInitialized;

            extern uint32_t placementAddress;

            extern void Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly);

        }
    }
}

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);
extern "C" void kfree_(void* ptr);
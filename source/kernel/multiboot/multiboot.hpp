#pragma once

#include <stdint.h>

#include "../../c_helpers/multiboot.h"

namespace Kernel
{
    namespace Multiboot
    {
        // Take a look here: https://github.com/Phil0nator/OS/blob/main/src/impl/kernel/multiboot_loader.c
        namespace MemoryMap
        {
            struct MemoryChunk
            {
                uint32_t start;
                uint32_t end;
            };

            struct PhysicalMemoryArea
            {
                MemoryChunk chunk;
                uint32_t type;
            };

            extern PhysicalMemoryArea globalMemoryList[24];
            extern uint32_t globalMemoryList_size;

            extern uint32_t heap_start;
            extern uint32_t heap_end;
        }

        extern void Load(uint32_t addr, uint32_t magic);
    }
}
#include "multiboot.hpp"

#include "../memory/kheap/kheap.hpp"

namespace Kernel
{
    namespace Multiboot
    {
        namespace MemoryMap
        {
            PhysicalMemoryArea globalMemoryList[24];
            uint32_t globalMemoryList_size = 0;

            uint32_t heap_start = 0;
            uint32_t heap_end = 0;
        }
    }
}

extern uint32_t linkerld_startofkernel;

void Kernel::Multiboot::Load(uint32_t addr, uint32_t magic)
{
    
}
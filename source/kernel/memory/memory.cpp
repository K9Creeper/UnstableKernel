#include "memory.hpp"

#include "../multiboot/multiboot.hpp"

namespace Kernel{
    namespace Memory{
        namespace Info{
            uint32_t kernel_start = 0;
            uint32_t kernel_end = 0;

            uint32_t mem_lower = 0;
            uint32_t mem_upper = 0;

            uint32_t kheap_start = 0;
            uint32_t kheap_end = 0;
            uint32_t kheap_max_address = 0;
        }

        

    }
}

extern uint32_t linkerld_startofkernel;
extern uint32_t linkerld_endofkernel;

void Kernel::Memory::InitMemInfo(){
    Info::kernel_start = reinterpret_cast<uint32_t>(&linkerld_startofkernel);
    Info::kernel_end =  reinterpret_cast<uint32_t>(&linkerld_endofkernel);
    
    multiboot_tag_basic_meminfo* basic_mem_info = Kernel::Multiboot::GetBasicMemInfo();

    // amount of lower memory available, in kilobytes. Lower memory is typically the first 640 KB of physical memory
    Info::mem_lower = basic_mem_info->mem_lower;

    // amount of upper memory available; starts just above the 1 MB mark and excludes reserved areas
    Info::mem_upper = basic_mem_info->mem_upper;

}
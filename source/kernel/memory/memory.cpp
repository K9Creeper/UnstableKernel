/// ----------
/// memory.cpp
/// @brief This file defines the variables for holding kernel memory information, aswell as defining
/// the function to retrive some kernel memory information.

#include "memory.hpp"

#include "../multiboot/multiboot.hpp"

namespace Kernel{
    namespace Memory{
        namespace Info{
            uint32_t kernel_start = 0;
            uint32_t kernel_end = 0;
            
            uint32_t pmm_size = 0;

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

    // For now a set of 1 GB
    Info::pmm_size = 0x40000000;
}
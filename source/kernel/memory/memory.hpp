/// ----------
/// memory.hpp
/// @brief This file declares the variables for holding kernel memory information, aswell as declareing
/// the function to retrive some kernel memory information.

#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace Info{
            extern uint32_t kernel_start;
            extern uint32_t kernel_end;

            extern uint32_t pmm_size;

            extern uint32_t kheap_start;
            extern uint32_t kheap_end;
            extern uint32_t kheap_max_address;
        }

        extern void InitMemInfo();

    }
}
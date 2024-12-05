#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace Info{
            extern uint32_t mem_lower;
            extern uint32_t mem_upper;

            extern uint32_t kernel_start;
            extern uint32_t kernel_end;

            extern uint32_t kheap_start;
            extern uint32_t kheap_end;
            extern uint32_t kheap_max_address;
        }

        extern void InitMemInfo();

    }
}
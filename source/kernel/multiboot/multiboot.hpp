#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace Multiboot
    {
        extern uint32_t mb_info;
        extern uint32_t mb_magic;

        extern uint32_t GetPhysicalMemorySize();
    }
}
#pragma once

#include <stdint.h>

#define KERNEL_MEMORY_GDT_ENTRYCOUNT 5

namespace Kernel
{
    namespace Memory
    {
        namespace GDT
        {
            extern void Init();
            extern void Install();
        }
    }
}
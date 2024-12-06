/// ---------------------------
/// global_descriptor_table.hpp
/// @brief This file declares the needed function to setup the Global Descriptor Table.

#pragma once

#include <stdint.h>

#define KERNEL_MEMORY_GDT_ENTRYCOUNT 8

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

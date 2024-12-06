/// ------------------------------
/// interrupt_descriptor_table.hpp
/// @brief This file declares the needed function to setup the Global Descriptor Table.

#pragma once

namespace Kernel
{
    namespace Memory
    {
        namespace IDT
        {
            extern void Init();
            extern void Install();
        }
    }
}
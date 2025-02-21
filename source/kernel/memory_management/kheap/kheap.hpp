/// ---------
/// kheap.hpp
/// @brief This file declates the heap of the kernel.

#pragma once

#include "../../../memory_management/heap.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        extern void* current;
        extern Heap kheap;
    }
}
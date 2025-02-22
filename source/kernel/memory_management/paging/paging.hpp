/// ----------
/// paging.hpp
/// @brief This file declares the functions and structures for kernel paging.

#pragma once

#include <stdint.h>

#include "../../../memory_management/paging.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        extern Paging pManager;

        extern PageDirectory* current;
        extern Paging* currentManager;

        extern void InitPaging();
    }
}
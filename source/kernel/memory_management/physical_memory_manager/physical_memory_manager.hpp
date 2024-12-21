/// ---------------------------
/// physical_memory_manager.hpp
/// @brief This file declares the functions and structures to manage physical memory.

#pragma once

#include <stdint.h>

#include "pmm_bitmap.hpp"

namespace Kernel{
    namespace MemoryManagement{
        namespace PMM{
            extern PMMBitmap bitmap;

            extern void Init(uint32_t mem_size);

            extern uint32_t AllocateBlock();
            extern void FreeBlock(uint32_t blockNum);

            extern uint32_t GetFirstFreeBlock(); 
        }
    }
}
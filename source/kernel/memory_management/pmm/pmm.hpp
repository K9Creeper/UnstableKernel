#pragma once

#include "../../../cpphelpers/bitmap.hpp"

namespace Kernel{
    namespace MemoryManagement{
        namespace PMM{
            extern Bitmap<uint8_t> bitmap;

            extern void Init();

            extern uint32_t BlindAllocBlock();
            extern void FreeBlock(uint32_t blk_num);
        }
    }
}
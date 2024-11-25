#pragma once

#include <stdint.h>

#define PHYSICAL_MEMORY_MANAGER_BLOCK_SIZE 4096
#define PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET 8

namespace Kernel{
    namespace Memory{
        namespace PMM{
            extern uint8_t* bitmap;
            extern uint8_t* mem_start;
            extern uint32_t total_blocks;
            extern uint32_t bitmap_size;

            extern void Init(uint32_t mem_size);

            extern uint32_t AllocateBlock();
            extern void FreeBlock(uint32_t blk_num);
        }
    }
}
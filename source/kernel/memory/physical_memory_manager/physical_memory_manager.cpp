#include "physical_memory_manager.hpp"

#include "../../../c_helpers/memory.h"

#define PHYSICAL_MEMORY_MANAGER_BLOCK_ALIGN(addr) (((addr) & 0xFFFFF000) + 0x1000)

extern uint32_t linkerld_endofprogram;

namespace Kernel{
    namespace Memory{
        namespace PMM{
            uint8_t* bitmap = (uint8_t*)(&linkerld_endofprogram);
            uint8_t* mem_start;
            uint32_t total_blocks;
            uint32_t bitmap_size;
        }
    }
}

uint32_t PMM_First_Free_Block() {
    uint32_t i;
    for(i = 0; i < Kernel::Memory::PMM::total_blocks; i++) {
        if(!((Kernel::Memory::PMM::bitmap[i / PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET] >> (i % PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET)) & 0x1))
            return i;
    }
    return static_cast<uint32_t>(-1);
}

void Kernel::Memory::PMM::Init(uint32_t mem_size){
    Kernel::Memory::PMM::total_blocks = mem_size / PHYSICAL_MEMORY_MANAGER_BLOCK_SIZE;

    Kernel::Memory::PMM::bitmap_size = Kernel::Memory::PMM::total_blocks / PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET;

    if(Kernel::Memory::PMM::bitmap_size * PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET < Kernel::Memory::PMM::total_blocks)
        Kernel::Memory::PMM::bitmap_size++;

    memset(Kernel::Memory::PMM::bitmap, 0, Kernel::Memory::PMM::bitmap_size);
    Kernel::Memory::PMM::mem_start = reinterpret_cast<uint8_t*>(PHYSICAL_MEMORY_MANAGER_BLOCK_ALIGN(reinterpret_cast<uint32_t>(Kernel::Memory::PMM::bitmap + Kernel::Memory::PMM::bitmap_size)));
}

uint32_t Kernel::Memory::PMM::AllocateBlock(){
    uint32_t free_block = PMM_First_Free_Block();

    bitmap[free_block / PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET] = (bitmap[free_block / PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET] | (1 << (free_block % PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET)));

    return free_block;
}

void Kernel::Memory::PMM::FreeBlock(uint32_t blk_num){
    bitmap[blk_num / PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET] = (bitmap[blk_num / PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET] & (~(1 << (blk_num % PHYSICAL_MEMORY_MANAGER_BLOCKS_PER_BUCKET))));
}
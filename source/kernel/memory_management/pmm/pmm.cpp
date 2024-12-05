#include "pmm.hpp"

#include "../kheap/kheap.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace PMM
        {
            Bitmap<uint8_t> bitmap;
        }
    }
}

void Kernel::MemoryManagement::PMM::Init()
{
    uint32_t tmp = Kernel::MemoryManagement::KHeap::Early::pkmalloc_(PMM::bitmap.GetMemSize());
    bitmap.RePlace(reinterpret_cast<void *>(tmp), 0);
}

static uint32_t FreeFirstBlock() {
    uint32_t total_blocks = Kernel::MemoryManagement::PMM::bitmap.GetSize();
    const uint32_t& sizeof_type_t = Kernel::MemoryManagement::PMM::bitmap.GetTypeSize();
    for(uint32_t i = 0; i < total_blocks; i++) {
        if(!((Kernel::MemoryManagement::PMM::bitmap[i / sizeof_type_t] >> (i % sizeof_type_t)) & 0x1))
            return i;
    }
    
    // Out of free blocks...
    // not good.
    return (uint32_t) -1;
}

uint32_t Kernel::MemoryManagement::PMM::BlindAllocBlock(){
    uint32_t free_block = FreeFirstBlock();
    const uint32_t& sizeof_type_t = bitmap.GetTypeSize();

    bitmap[free_block / sizeof_type_t] = bitmap[free_block / sizeof_type_t] | (1 << (free_block % sizeof_type_t));

    return free_block;
}

void Kernel::MemoryManagement::PMM::FreeBlock(uint32_t blk_num){
    const uint32_t& sizeof_type_t = bitmap.GetTypeSize();
    bitmap[blk_num / sizeof_type_t] = bitmap[blk_num / sizeof_type_t] & (~(1 << (blk_num % sizeof_type_t)));
}
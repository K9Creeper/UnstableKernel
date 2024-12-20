/// ---------------------------
/// physical_memory_manager.cpp
/// @brief

#include "physical_memory_manager.hpp"

#include "../../../chelpers/memory.h"

#include "../kheap/kheap.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace PMM
        {
            PMMBitmap bitmap;
        }
    }
}

extern "C" void printf(const char *format, ...);

void PMMBitmap::Clear()
{
    memset(reinterpret_cast<uint8_t *>(array), 0, memSize); // fill with zeros
}

void Kernel::MemoryManagement::PMM::Init(uint32_t mem_size)
{
    bitmap.Create();

    const int count = mem_size / 0x1000;
    const int size = (count / bitmap.GetBytesPerEntry());

    bitmap.RePlace(reinterpret_cast<uint32_t *>(Kernel::MemoryManagement::KHeap::Early::pkmalloc_(size)), size);

    bitmap.Clear();
}

uint32_t Kernel::MemoryManagement::PMM::AllocateBlock(){
    uint32_t free_block = GetFirstFreeBlock();

    uint32_t index = bitmap.IndexFromBit(free_block);
    uint32_t offset = bitmap.OffsetFromBit(free_block);

    bitmap[index] |= (0x1 << offset);

    return free_block;
}

void Kernel::MemoryManagement::PMM::FreeBlock(uint32_t blockNum){
    uint32_t index = bitmap.IndexFromBit(blockNum);
    uint32_t offset = bitmap.OffsetFromBit(blockNum);

    bitmap[index] &= ~(0x1 << offset);
}

uint32_t Kernel::MemoryManagement::PMM::GetFirstFreeBlock(){
    for(uint32_t i = 0; i < bitmap.GetCount(); i++) {
        if(!((bitmap[bitmap.IndexFromBit(i)] >> (bitmap.OffsetFromBit(i))) & 0x1))
            return i;
    }
    return static_cast<uint32_t>(-1);
}
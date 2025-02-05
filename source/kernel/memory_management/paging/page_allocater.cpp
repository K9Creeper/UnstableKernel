/// ------------------
/// page_allocater.cpp
/// This file declares the function(s) of handling compile time allocated page directories

#include "page_allocater.hpp"

#include "../../../chelpers/memory.h"

#include "../kheap/kheap.hpp"

#define DATA_LENGTH 256

void PageAllocater::PreInit()
{
    raw_data[0].directory = reinterpret_cast<PageDirectory*>(Kernel::MemoryManagement::KHeap::Early::pkmalloc_(sizeof(PageDirectory), true));
}

void PageAllocater::Init(){
    int i;
    for(i = 1; i < DATA_LENGTH; i++)
    {
        raw_data[i].directory = reinterpret_cast<PageDirectory*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageDirectory), true));
    }
}

PageDirectory *PageAllocater::AllocatePageDirectory(bool PROTECTED)
{
    int i;
    for (i = 0; i < DATA_LENGTH; i++)
    {
        if (raw_data[i].bUsed || raw_data[i].PROTECT)
            continue;

        raw_data[i].bUsed = true;
        raw_data[i].PROTECT = PROTECTED;

        memset(reinterpret_cast<uint8_t *>(raw_data[i].directory), reinterpret_cast<uint8_t *>(0), sizeof(PageDirectory));

        return raw_data[i].directory;
    }

    // Uh oh, out of page directories
    return nullptr;
}

void PageAllocater::FreePageDirectory(PageDirectory *loc)
{
    int i;
    for (i = 0; i < DATA_LENGTH; i++)
    {
        if (!raw_data[i].bUsed || raw_data[i].PROTECT || loc != raw_data[i].directory)
            continue;

        raw_data[i].bUsed = false;
        raw_data[i].PROTECT = false;

        memset(reinterpret_cast<uint8_t *>(raw_data[i].directory), reinterpret_cast<uint8_t *>(0), sizeof(PageDirectory));

        break;
    }
}

PageAllocater PageDirectoryAllocater;
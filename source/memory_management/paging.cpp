/// ----------
/// paging.cpp
/// @brief This file defines the functions for system paging.

#include "paging.hpp"

#include "../chelpers/memory.h"

#include "heap.hpp"

#include "../kernel/memory_management/physical_memory_manager/physical_memory_manager.hpp"
#include "../kernel/memory_management/paging/paging.hpp"

// boot.s
extern "C" PageDirectory *pageDirectory;

namespace Kernel
{
    namespace MemoryManagement
    {
        PageDirectory *current = pageDirectory;
        Paging* currentManager = nullptr;
    }
}

static bool bEnabled = false;
static bool bPSEEnabled = true;

void Paging::Init(uint32_t directoryLoc, Heap *heap, bool shouldClear)
{
    if (!directoryLoc || !heap)
        return;

    dir = reinterpret_cast<PageDirectory *>(directoryLoc);

    if (shouldClear)
        memset(reinterpret_cast<uint8_t *>(dir), 0, sizeof(PageDirectory));

    this->heap = heap;

    bInitialzed = true;
}

bool Paging::isEnabled() const
{
    return bEnabled;
}

bool Paging::isInitialzed() const
{
    return bInitialzed;
}

PageDirectory *Paging::GetDirectory() const
{
    return dir;
}

void Paging::AllocateRegion(uint32_t start, uint32_t end, bool identity, bool isKernel, bool isWritable)
{
    uint32_t i = start;
    while (i <= end)
    {
        if (identity)
            AllocatePage(i, i / 0x1000, isKernel, isWritable);
        else
            AllocatePage(i, 0, isKernel, isWritable);

        i += 0x1000;
    }
}

void Paging::DisablePSEReg()
{
    if (!bPSEEnabled)
        return;
    uint32_t cr4;
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 &= 0xffffffef;
    asm volatile("mov %0, %%cr4" ::"r"(cr4));

    bPSEEnabled = false;
}

void Paging::EnablePaging()
{
    if (bEnabled)
        return;

    // will do its thang
    DisablePSEReg();

    uint32_t cr0;

    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" ::"r"(cr0));

    bEnabled = true;
}

void Paging::SwitchToDirectory(bool isPhysical, uint32_t cr3)
{
    uint32_t t;

    if (cr3)
    {
        if (!isPhysical)
        {
            t = Virtual2Phyiscal(cr3, pageDirectory);
        }
        else
            t = cr3;
    }
    else
    {
        if (!isPhysical)
            t = Virtual2Phyiscal(dir, pageDirectory);
        else
            t = reinterpret_cast<uint32_t>(dir);
    }
    
    asm volatile("mov %0, %%cr3" ::"r"(t));

    Kernel::MemoryManagement::currentManager = this;
    Kernel::MemoryManagement::current = dir;
}

uint32_t Paging::Virtual2Phyiscal(uint32_t virtual_address, PageDirectory *diff)
{
    PageDirectory *d = dir;
    if (diff)
    {
        d = diff;
    }

    if (!bEnabled)
    {
        return (virtual_address - 0xC0000000);
    }

    uint32_t pageDirIdx = virtual_address >> 22;
    uint32_t pageTblIdx = (virtual_address >> 12) & 0x3ff;
    uint32_t pageFrameOffset = (virtual_address & 0xfff);

    if (!d->ref_tables[pageDirIdx])
    {
        return 0;
    }

    PageTable *table = d->ref_tables[pageDirIdx];
    if (!table->pages[pageTblIdx].present)
    {
        return 0;
    }

    return (table->pages[pageTblIdx].frame << 12) + pageFrameOffset;
}

void Paging::SwapHeap(Heap *heap)
{
    this->heap = heap;
}

Heap* Paging::GetHeap()const{
    return heap;
}

void Paging::AllocatePage(uint32_t virtual_address, uint32_t frame, bool isKernel, int isWritable, PageDirectory *other)
{
    PageDirectory *pd = dir;
    if (other)
        pd = other;

    PageTable *table = nullptr;
    if (!pd)
        return;

    uint32_t pageDirIdx = virtual_address >> 22;
    uint32_t pageTblIdx = (virtual_address >> 12) & 0x3ff;

    table = pd->ref_tables[pageDirIdx];
    if (!table)
    {
        table = heap->malloc_(sizeof(PageTable), true);

        memset(reinterpret_cast<uint8_t *>(table), reinterpret_cast<uint8_t *>(0), sizeof(PageTable));

        uint32_t t = Virtual2Phyiscal(table, Kernel::MemoryManagement::pManager.GetDirectory());
        pd->tables[pageDirIdx].frame = t >> 12;
        pd->tables[pageDirIdx].present = 1;
        pd->tables[pageDirIdx].rw = 1;
        pd->tables[pageDirIdx].user = 1;
        pd->tables[pageDirIdx].page_size = 0;

        pd->ref_tables[pageDirIdx] = table;
    }

    if (!table->pages[pageTblIdx].present)
    {
        uint32_t t;
        if (frame)
            t = frame;
        else
            t = Kernel::MemoryManagement::PMM::AllocateBlock();
        table->pages[pageTblIdx].frame = t;
        table->pages[pageTblIdx].present = 1;
        table->pages[pageTblIdx].rw = 1;
        table->pages[pageTblIdx].user = 1;
    }
}

void Paging::FreePage(uint32_t virtual_address, bool bFree)
{
    if (dir == pageDirectory)
        return;

    uint32_t pageDirIdx = virtual_address >> 22;
    uint32_t pageTblIdx = (virtual_address >> 12) & 0x3ff;

    if (!dir->ref_tables[pageDirIdx])
    {
        return;
    }

    PageTable *table = dir->ref_tables[pageDirIdx];
    if (!table->pages[pageTblIdx].present)
    {
        return;
    }

    if (bFree)
        Kernel::MemoryManagement::PMM::FreeBlock(table->pages[pageTblIdx].frame);

    table->pages[pageTblIdx].present = 0;
    table->pages[pageTblIdx].frame = 0;
}

PageTable *Paging::CopyPageTable(PageDirectory *dst_page_dir, uint32_t page_dir_idx, PageTable *src)
{
    PageTable *table = reinterpret_cast<PageTable *>(heap->malloc_(sizeof(PageTable), true));
    for (int i = 0; i < 1024; i++)
    {
        if (!table->pages[i].frame)
            continue;

        uint32_t src_virtual_address = (page_dir_idx << 22) | (i << 12) | (0);

        uint32_t dst_virtual_address = src_virtual_address;

        uint32_t tmp_virtual_address = 0;
        AllocatePage(dst_virtual_address, 0, false, true, dst_page_dir);
        AllocatePage(tmp_virtual_address, Virtual2Phyiscal(dst_virtual_address, dst_page_dir), false, true);
        if (src->pages[i].present)
            table->pages[i].present = 1;
        if (src->pages[i].rw)
            table->pages[i].rw = 1;
        if (src->pages[i].user)
            table->pages[i].user = 1;
        if (src->pages[i].accessed)
            table->pages[i].accessed = 1;
        if (src->pages[i].dirty)
            table->pages[i].dirty = 1;
        memcpy(reinterpret_cast<uint8_t *>(tmp_virtual_address), reinterpret_cast<uint8_t *>(src_virtual_address), 0x1000);
        FreePage(tmp_virtual_address, false);
    }
    return table;
}

void Paging::CopyDirectory(PageDirectory *dst, PageDirectory * src)
{
    PageDirectory* c = dir;

    if(src)
        c = src;

    for (uint32_t i = 0; i < 1024; i++)
    {
        if (Kernel::MemoryManagement::pManager.dir->ref_tables[i] == c->ref_tables[i])
        {
            dst->tables[i] = c->tables[i];
            dst->ref_tables[i] = c->ref_tables[i];
        }
        else
        {
            dst->ref_tables[i] = CopyPageTable(dst, i, dir->ref_tables[i]);
            uint32_t phys = Virtual2Phyiscal(reinterpret_cast<uint32_t>(dst->ref_tables[i]), c);
            dst->tables[i].frame = phys >> 12;
            dst->tables[i].user = 1;
            dst->tables[i].rw = 1;
            dst->tables[i].present = 1;
        }
    }
}
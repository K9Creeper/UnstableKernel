/// ----------
/// paging.cpp
/// @brief This file defines the functions for kernel paging.

#include "paging.hpp"

#include "../../../chelpers/memory.h"

#include "../../memory/memory.hpp"

#include "../physical_memory_manager/physical_memory_manager.hpp"
#include "../kheap/kheap.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace Paging
        {
            PageDirectory *kernelDirectory = nullptr;

            bool bPSEEnabled = true;

            bool bEnabled = false;
            bool bInitialzed = false;
        }
    }
}

// from boot
extern "C" PageDirectory *pageDirectory;

void Kernel::MemoryManagement::Paging::Init()
{
    kernelDirectory = reinterpret_cast<PageDirectory*>(KHeap::Early::pkmalloc_(sizeof(PageDirectory), true));
    memset(reinterpret_cast<uint8_t *>(kernelDirectory), 0, sizeof(PageDirectory));

    uint32_t i = 0xC0000000; // + 4mb
    while (i < 0xC0000000 + 0x400000 + 0x1000)
    {
        AllocatePage(kernelDirectory, i, 0, true, true);
        i += 0x1000;
    }

    SwitchDirectory(kernelDirectory, 0);
    EnablePaging();

    Kernel::MemoryManagement::Paging::bInitialzed = true;

    i = 0;
    while (i < 0x100000 + 0x1000)
    {
        AllocatePage(kernelDirectory, i, i / 0x1000, true, true);
        i += 0x1000;
    }
}

void DisablePSEReg()
{
    if (!Kernel::MemoryManagement::Paging::bPSEEnabled)
        return;
    uint32_t cr4;
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 &= 0xffffffef;
    asm volatile("mov %0, %%cr4" ::"r"(cr4));
}

void Kernel::MemoryManagement::Paging::EnablePaging()
{
    // will do its thang
    DisablePSEReg();

    uint32_t cr0;

    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" ::"r"(cr0));

    bEnabled = true;
}

void Kernel::MemoryManagement::Paging::SwitchDirectory(PageDirectory *dir, bool isPhysical)
{
    uint32_t t;
    if (!isPhysical)
        t = Virtual2Phyiscal(pageDirectory, reinterpret_cast<uint32_t>(dir));
    else
        t = reinterpret_cast<uint32_t>(dir);
    asm volatile("mov %0, %%cr3" ::"r"(t));
}

uint32_t Kernel::MemoryManagement::Paging::Virtual2Phyiscal(PageDirectory *dir, uint32_t virtual_address)
{
    if (!bEnabled)
    {
        return (virtual_address - 0xC0000000);
    }

    uint32_t pageDirIdx = virtual_address >> 22;
    uint32_t pageTblIdx = (virtual_address >> 12) & 0x3ff;
    uint32_t pageFrameOffset = (virtual_address & 0xfff);

    if (!dir->ref_tables[pageDirIdx])
    {
        return 0;
    }

    PageTable *table = dir->ref_tables[pageDirIdx];
    if (!table->pages[pageTblIdx].present)
    {
        return 0;
    }

    return (table->pages[pageTblIdx].frame << 12) + pageFrameOffset;
}

void Kernel::MemoryManagement::Paging::AllocatePage(PageDirectory *dir, uint32_t virtual_address, uint32_t frame, bool isKernel, int isWritable)
{
    PageTable *table = nullptr;
    if (!dir)
        return;

    uint32_t pageDirIdx = virtual_address >> 22;
    uint32_t pageTblIdx = (virtual_address >> 12) & 0x3ff;

    table = dir->ref_tables[pageDirIdx];
    if (!table)
    {
        table = Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageTable), true);

        memset(reinterpret_cast<uint8_t *>(table), reinterpret_cast<uint8_t*>(0), sizeof(PageTable));

        uint32_t t = Virtual2Phyiscal(kernelDirectory, table);
        dir->tables[pageDirIdx].frame = t >> 12;
        dir->tables[pageDirIdx].present = 1;
        dir->tables[pageDirIdx].rw = 1;
        dir->tables[pageDirIdx].user = 1;
        dir->tables[pageDirIdx].page_size = 0;

        dir->ref_tables[pageDirIdx] = table;
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

void Kernel::MemoryManagement::Paging::FreePage(PageDirectory *dir, uint32_t virtual_address, bool bFree)
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

PageTable* CopyPageTable(PageDirectory* src_page_dir, PageDirectory* dst_page_dir, uint32_t page_dir_idx, PageTable * src) {
    PageTable* table = reinterpret_cast<PageTable*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageTable), true));
    for(int i = 0; i < 1024; i++) {
        if(!table->pages[i].frame)
            continue;

        uint32_t src_virtual_address = (page_dir_idx << 22) | (i << 12) | (0);

        uint32_t dst_virtual_address = src_virtual_address;

        uint32_t tmp_virtual_address = 0;
        Kernel::MemoryManagement::Paging::AllocatePage(dst_page_dir, dst_virtual_address, 0, false, true);
        Kernel::MemoryManagement::Paging::AllocatePage(src_page_dir, tmp_virtual_address, Kernel::MemoryManagement::Paging::Virtual2Phyiscal(dst_page_dir, dst_virtual_address), false, true);
        if (src->pages[i].present) table->pages[i].present = 1;
        if (src->pages[i].rw)      table->pages[i].rw = 1;
        if (src->pages[i].user)    table->pages[i].user = 1;
        if (src->pages[i].accessed)table->pages[i].accessed = 1;
        if (src->pages[i].dirty)   table->pages[i].dirty = 1;
        memcpy(reinterpret_cast<uint8_t*>(tmp_virtual_address), reinterpret_cast<uint8_t*>(src_virtual_address), 0x1000);
        Kernel::MemoryManagement::Paging::FreePage(src_page_dir, tmp_virtual_address, false);
    }
    return table;
}

void Kernel::MemoryManagement::Paging::CopyDirectory(PageDirectory* src, PageDirectory* dst){
    for(uint32_t i = 0; i < 1024; i++) {
        if(kernelDirectory->ref_tables[i] == src->ref_tables[i]) {
            dst->tables[i] = src->tables[i];
            dst->ref_tables[i] = src->ref_tables[i];
        }
        else {
            dst->ref_tables[i] = CopyPageTable(src, dst, i, src->ref_tables[i]);
            uint32_t phys = Kernel::MemoryManagement::Paging::Virtual2Phyiscal(src, reinterpret_cast<uint32_t>(dst->ref_tables[i]));
            dst->tables[i].frame = phys >> 12;
            dst->tables[i].user = 1;
            dst->tables[i].rw = 1;
            dst->tables[i].present = 1;
        }
    }
}
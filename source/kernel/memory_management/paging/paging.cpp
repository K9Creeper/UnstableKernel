#include "paging.hpp"

#include "../../../chelpers/memory.h"

#include "../../memory/memory.hpp"

#include "../kheap/kheap.hpp"

#include "paging_bitmap.hpp"

extern "C" void printf(const char *format, ...);

void PagingBitmap::Clear()
{
    memset(reinterpret_cast<uint8_t *>(array), 0, memSize); // fill with zeros
}

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace Paging
        {
            bool bIntialized = false;
            bool bEnabled = false;

            PageDirectory *currentDirectory;
            PageDirectory *kernelDirectory;

            PagingBitmap bitmap;
        }
    }
}

Kernel::MemoryManagement::Paging::Page *Kernel::MemoryManagement::Paging::GetPageEntry(uint32_t address, Kernel::MemoryManagement::Paging::PageDirectory *dir, bool sMake, uint32_t *out_physical_address)
{
    // turn the address into an index
    uint32_t frame = address / 0x1000;

    // Find the page table containing this address
    uint32_t table_idx = frame / 1024;

    if (dir->tables[table_idx]) // page-table is already assigned
    {
        uint32_t page_idx = frame % 1024;

        return &dir->tables[table_idx]->pages[page_idx];
    }
    else if (sMake)
    {
        return MakePageEntry(address, dir);
    }

    return 0;
}

Kernel::MemoryManagement::Paging::Page *Kernel::MemoryManagement::Paging::MakePageEntry(uint32_t address, PageDirectory *dir)
{
    // address to an index
    uint32_t frame = address / 0x1000;

    // find the page table
    uint32_t table_idx = frame / 1024;
    uint32_t page_idx = frame % 1024;

    uint32_t physical_address;

    // retrieve a memory block which is page aligned, aswell as its physical address
    uint32_t virtual_address = Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Kernel::MemoryManagement::Paging::PageTable), 1, &physical_address);
    memset(reinterpret_cast<unsigned char *>(virtual_address), 0, 0x1000); // fill with zeros

    // virtual address
    dir->tables[table_idx] = reinterpret_cast<Kernel::MemoryManagement::Paging::PageTable *>(virtual_address);

    // physical address
    // set present, rw, and user-mode bits
    dir->tablePhysicals[table_idx] = physical_address | 0x7;

    // return what was made
    return &dir->tables[table_idx]->pages[page_idx];
}

static void SetFrame(uint32_t frame_address)
{
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = Kernel::MemoryManagement::Paging::bitmap.IndexFromBit(frame);
    uint32_t offset = Kernel::MemoryManagement::Paging::bitmap.OffsetFromBit(frame);
    Kernel::MemoryManagement::Paging::bitmap[index] |= 0x1 << offset;
}

static void ClearFrame(uint32_t frame_address)
{
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = Kernel::MemoryManagement::Paging::bitmap.IndexFromBit(frame);
    uint32_t offset = Kernel::MemoryManagement::Paging::bitmap.OffsetFromBit(frame);
    Kernel::MemoryManagement::Paging::bitmap[index] &= ~(0x1 << offset);
}

static bool FirstFrame(uint32_t &out)
{
    // loop through bitmap
    for (uint32_t i = 0; i < Kernel::MemoryManagement::Paging::bitmap.GetMemSize(); i += 1)
    {
        // exit if nothing free
        if (Kernel::MemoryManagement::Paging::bitmap[i] != 0xFFFFFFFF)
        {
            // loop through bits
            for (uint32_t j = 0; j < 32; j += 1)
            {
                uint32_t test = 0x1 << j;

                // if its free yoink it, huzzah!
                if (!(Kernel::MemoryManagement::Paging::bitmap[i] & test))
                {
                    out = i * 32 + j;
                    return true;
                }
            }
        }
    }

    out = -1;
    return false;
}

void Kernel::MemoryManagement::Paging::AllocateFrame(Kernel::MemoryManagement::Paging::Page *page, bool is_kernel, bool is_writeable)
{
    if (page->frame == 0)
    {
        uint32_t index = 0;

        if (!FirstFrame(index))
        {
            printf("Out of pages\n");
            // no free pages
            for (;;)
            {
                asm volatile("hlt");
            }
        }

        // initialize a frame
        SetFrame(index * 0x1000);

        // add our flags
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame = index;
    }
}

void SetupPMM(uint32_t mem_size)
{
    Kernel::MemoryManagement::Paging::bitmap.Create();

    const int count = mem_size / 0x1000;
    const int size = (count / Kernel::MemoryManagement::Paging::bitmap.GetBytesPerEntry());

    Kernel::MemoryManagement::Paging::bitmap.RePlace(reinterpret_cast<uint32_t *>(Kernel::MemoryManagement::KHeap::kmalloc_(size)), size);

    Kernel::MemoryManagement::Paging::bitmap.Clear();
}

void Kernel::MemoryManagement::Paging::Init(uint32_t mem_size)
{
    SetupPMM(mem_size);

    uint32_t tmp = Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageDirectory), true);
    memset(reinterpret_cast<uint8_t *>(tmp), 0, sizeof(PageDirectory)); // fill with zeros

    kernelDirectory = reinterpret_cast<PageDirectory *>(tmp);

    currentDirectory = kernelDirectory;

    for (uint32_t j = KHEAP_START; j < KHEAP_START + KHEAP_INITIAL_SIZE; j += 0x1000)
    {
        GetPageEntry(j, currentDirectory, 1);
    }

    for (uint32_t j = 0; j < Kernel::MemoryManagement::KHeap::Early::nextPlacementAddress + 0x1000; j += 0x1000)
    {
        AllocateFrame(GetPageEntry(j, currentDirectory, 1), 0, 0);
    }

    for (uint32_t j = KHEAP_START; j < KHEAP_START + KHEAP_INITIAL_SIZE; j += 0x1000)
    {
        AllocateFrame(GetPageEntry(j, currentDirectory, 1), 0, 1);
    }

    SwitchPageDirectory(kernelDirectory);

    Enable();
}

void Kernel::MemoryManagement::Paging::Enable()
{
    if (bEnabled)
        return;
    uint32_t cr0;

    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" ::"r"(cr0));

    bEnabled = true;
}

void Kernel::MemoryManagement::Paging::SwitchPageDirectory(Kernel::MemoryManagement::Paging::PageDirectory *dir)
{
    Kernel::MemoryManagement::Paging::currentDirectory = dir;

    // CR3 = address of tablePhysicals
    asm volatile("mov %0, %%cr3" : : "r"(&dir->tablePhysicals));
}

void Kernel::MemoryManagement::Paging::FreeFrame(Kernel::MemoryManagement::Paging::Page *page)
{
    uint32_t frame = page->frame;

    if (!frame)
    {
        return; // frame was not previously allocated
    }
    else
    {
        ClearFrame(frame);
        page->frame = 0;
    }
}
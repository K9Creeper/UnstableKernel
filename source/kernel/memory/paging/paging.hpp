#pragma once

#include <stdint.h>

#define PAGING_COUNTS 1024
#define PAGE_SIZE 0x1000
#define PAGE_ALIGN_MASK 0xFFFFF000

namespace Kernel
{
    namespace Memory
    {
        /// 1:1 Paging Structure
        namespace Paging
        {
            struct PageEntry
            {
                uint32_t present : 1;  // present in memory
                uint32_t rw : 1;       // read-only / readwrite
                uint32_t user : 1;     // supervisor level
                uint32_t accessed : 1; // been accessed since last refresh?
                uint32_t dirty : 1;    // been written to since last refresh?
                uint32_t unused : 7;   // unused and reserved bits
                uint32_t frame : 20;   // frame address (shifted right 12 bits)
            };

            struct PageEntryTable
            {
                PageEntry pages[PAGING_COUNTS];
            };

            struct PageDirectory
            {
                PageEntryTable *tables[PAGING_COUNTS];
                uint32_t tablePhysicals[PAGING_COUNTS];
                uint32_t physicalAddress;
            };

            struct Frames
            {
                uint32_t *bitmap;
                uint32_t count;
            };

            extern PageDirectory *kernelDirectory;
            extern PageDirectory *currentDirectory;

            extern Frames frames;

            extern void Init(uint32_t heap_start, uint32_t framebuffer_start, uint32_t framebuffer_size);

            extern void Enable();
            extern void Disable();

            extern void SwitchPageDirectory(PageDirectory *dir);
            extern PageEntry *GetPageEntry(uint32_t address, PageDirectory *dir, bool sMake = false, uint32_t* out_physical_address = nullptr);
            extern PageEntry *MakePageEntry(uint32_t address, PageDirectory *dir);
            extern void AllocateFrame(PageEntry *page, bool is_kernel, bool is_writeable);
            extern void FreeFrame(PageEntry *page);
        }
    }
}
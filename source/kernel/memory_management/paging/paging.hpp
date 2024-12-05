#pragma once

#include <stdint.h>

#include "paging_bitmap.hpp"

//
// Target: Bitmap System
//

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace Paging
        {
            extern bool bIntialized;
            extern bool bEnabled;

            struct Page
            {
                uint32_t present : 1;  // present in memory
                uint32_t rw : 1;       // read-only / readwrite
                uint32_t user : 1;     // supervisor level
                uint32_t accessed : 1; // been accessed since last refresh?
                uint32_t dirty : 1;    // been written to since last refresh?
                uint32_t unused : 7;   // unused and reserved bits
                uint32_t frame : 20;   // frame address (shifted right 12 bits)
            };

            struct PageTable
            {
                Page pages[1024];
            };

            struct PageDirectory
            {
                PageTable *tables[1024];
                uint32_t tablePhysicals[1024];
                uint32_t physicalAddress;
            };

            extern PageDirectory *kernelDirectory;
            extern PageDirectory *currentDirectory;
            
            extern uint32_t* bitmap;
            extern uint32_t  bitmapCount;

            extern void Init(uint32_t mem_size);

            extern void Enable();
            extern void Disable();

            extern void SwitchPageDirectory(PageDirectory *dir);
            extern Page *GetPageEntry(uint32_t address, PageDirectory *dir, bool sMake = false, uint32_t* out_physical_address = nullptr);
            extern Page *MakePageEntry(uint32_t address, PageDirectory *dir);
            extern void AllocateFrame(Page *page, bool is_kernel, bool is_writeable);
            extern void FreeFrame(Page *page);

        }
    }
}
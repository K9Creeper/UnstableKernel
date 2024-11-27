#pragma once

#include <stdint.h>

#define PAGING_COUNTS 1024
#define PAGE_SIZE 0x1000
#define PAGE_ALIGN_MASK 0xFFFFF000

namespace Kernel{
    namespace Memory{
        namespace Paging{
            struct PageEntry
            {
	            uint32_t present    :  1;  // present in memory
	            uint32_t rw         :  1;  // read-only / readwrite
	            uint32_t user       :  1;  // supervisor level
	            uint32_t accessed   :  1;  // been accessed since last refresh?
	            uint32_t dirty      :  1;  // been written to since last refresh?
	            uint32_t unused     :  7;  // unused and reserved bits
	            uint32_t frame      : 20;  // frame address (shifted right 12 bits)
            };

            struct PageEntryTable{
                PageEntry pages[PAGING_COUNTS];
            };

            struct PageDirectory{
                PageEntryTable* tables[PAGING_COUNTS];
                uint32_t tablePhysicals[PAGING_COUNTS];
                uint32_t physicalAddress;
            };

            struct Frames{
                uint32_t* bitmap;
                uint32_t count;
            };

            static PageDirectory* kernelDirectory = nullptr;
            static PageDirectory* currentDirectory = nullptr;

            static Frames frames;

            extern void Init();

            extern void Enable();

            extern void SwitchPageDirectory(PageDirectory* dir);
            extern PageEntry* GetPageEntry(uint32_t address, int make, PageDirectory* dir);

            extern void AllocateFrame(PageEntry *page, bool is_kernel, bool is_writeable);
            extern void FreeFrame(PageEntry *page);
        }
    }
}
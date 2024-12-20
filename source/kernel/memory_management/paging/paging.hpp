/// ----------
/// paging.hpp
/// @brief


#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace Paging
        {
            struct PageDirectoryEntry
            {
                unsigned int present : 1;
                unsigned int rw : 1;
                unsigned int user : 1;
                unsigned int w_through : 1;
                unsigned int cache : 1;
                unsigned int access : 1;
                unsigned int reserved : 1;
                unsigned int page_size : 1;
                unsigned int global : 1;
                unsigned int available : 3;
                unsigned int frame : 20;
            };

            struct PageEntry
            {
                unsigned int present : 1;
                unsigned int rw : 1;
                unsigned int user : 1;
                unsigned int reserved : 2;
                unsigned int accessed : 1;
                unsigned int dirty : 1;
                unsigned int reserved2 : 2;
                unsigned int available : 3;
                unsigned int frame : 20;
            };

            struct PageTable
            {
                PageEntry pages[1024];
            };

            struct PageDirectory
            {
                // The actual page directory entries(note that the frame number it stores is physical address)
                PageDirectoryEntry tables[1024];
                // We need a table that contains virtual address, so that we can actually get to the tables
                PageTable *ref_tables[1024];
            };

            extern bool bEnabled;
            extern bool bInitialzed;

            extern PageDirectory* kernelDirectory;

            extern void Init();

            extern void EnablePaging();
            extern void SwitchDirectory(PageDirectory* dir, bool isPhysical);
        
            extern uint32_t Virtual2Phyiscal(PageDirectory* dir, uint32_t virtual_address);

            extern void AllocatePage(PageDirectory * dir, uint32_t virtual_address, uint32_t frame, bool isKernel = false, int isWritable = false);
            extern void FreePage(PageDirectory * dir, uint32_t virtual_address, bool bFree);
        }
    }
}
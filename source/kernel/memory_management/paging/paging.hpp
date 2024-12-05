#pragma once

#include <stdint.h>

//
// Target: Bitmap System
//

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
                unsigned int frame : 20; // ( stores physical address)
            };

             struct Page
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
                Page pages[1024];
            };

            struct PageDirectory
            {
                // The actual page directory entries
                PageDirectoryEntry tables[1024];

                // contains virtual address in order to get to table
                PageTable *ref_tables[1024];
            };

            extern bool bIntialized;
            extern bool bEnabled;

            extern PageDirectory *kernelDirectory;
            extern PageDirectory *currentDirectory;

            extern void Init(uint32_t mem_size);
        }
    }
}
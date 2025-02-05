/// ----------
/// paging.hpp
/// @brief This file declares the functions and structures for kernel paging.

#pragma once

#include <stdint.h>

#include "../../../memory_management/paging.hpp"

namespace Kernel
{
    namespace MemoryManagement
    {
        namespace Paging
        {
            extern bool bEnabled;
            extern bool bInitialzed;

            extern PageDirectory* kernelDirectory;

            extern void Init();

            extern void EnablePaging();
            extern void SwitchDirectory(PageDirectory* dir, bool isPhysical);
        
            extern uint32_t Virtual2Phyiscal(PageDirectory* dir, uint32_t virtual_address);

            extern void AllocatePage(PageDirectory * dir, uint32_t virtual_address, uint32_t frame, bool isKernel = false, int isWritable = false);
            extern void FreePage(PageDirectory * dir, uint32_t virtual_address, bool bFree);

            extern void CopyDirectory(PageDirectory* in, PageDirectory* out);
        }
    }
}
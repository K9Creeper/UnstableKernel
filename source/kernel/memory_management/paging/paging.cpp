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
        Paging pManager; 
    }
}

void Kernel::MemoryManagement::InitPaging(){
    pManager.Init(Kernel::MemoryManagement::kheap.malloc_(sizeof(PageDirectory), true), &kheap);

    pManager.AllocateRegion(0xC0000000, 0xC0000000 + 0x400000, false, true, true);
    
    pManager.SwitchToDirectory(false);
    pManager.EnablePaging();

    pManager.AllocateRegion(0x0, 0x100000, true, true, true);
}
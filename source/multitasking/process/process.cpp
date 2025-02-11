/// -----------
/// process.cpp
/// @brief This file defines the functions of process class.

#include "process.hpp"

#include "../../kernel/memory_management/kheap/kheap.hpp"
#include "../../kernel/memory_management/paging/paging.hpp"

#include "../../chelpers/memory.h"
#include "../../chelpers/string.h"

static uint32_t proccessIdCounter = 0;

void Process::Init(char* filename, void* routine){
    memcpy(reinterpret_cast<uint8_t*>(this->filename), filename, strlen(filename));

    this->procId = proccessIdCounter++;

    this->regs.eflags = 0x206;

    this->pageDir = Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageDirectory), true);
    memset(reinterpret_cast<uint8_t*>(this->pageDir), 0, sizeof(PageDirectory));

    Kernel::MemoryManagement::Paging::CopyDirectory(Kernel::MemoryManagement::Paging::kernelDirectory, this->pageDir);

    if(routine)
    {
        this->regs.eip = reinterpret_cast<uint32_t>(routine);

        for(uint32_t a = 0xC0000000 - 4 * 0x1000; a < 0xC0000000; a++)
        {
            Kernel::MemoryManagement::Paging::AllocatePage(this->pageDir, a, 0, false, true);
        }

    }else{
        this->stack = 0xC0000000;
        this->regs.esp = (this->stack - 0x1000);
    }

    this->regs.cr3 = Kernel::MemoryManagement::Paging::Virtual2Phyiscal(Kernel::MemoryManagement::Paging::kernelDirectory, reinterpret_cast<uint32_t>(this->pageDir));
    this->state = ProcessState_Created;

    if(!routine)
    {
        asm volatile("mov $1, %eax");
        asm volatile("int $0x80");
    }
}
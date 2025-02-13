/// -----------
/// process.cpp
/// @brief This file defines the functions of process class.

#include "process.hpp"

#include "../../kernel/memory_management/kheap/kheap.hpp"
#include "../../kernel/memory_management/paging/paging.hpp"

#include "../../chelpers/memory.h"
#include "../../chelpers/string.h"

static uint32_t proccessIdCounter = 0;

void Process::Init(char *filename, void *routine)
{
    memcpy(reinterpret_cast<uint8_t *>(this->filename), filename, strlen(filename));

    this->procId = proccessIdCounter++;
    this->regs.eip = reinterpret_cast<uint32_t>(routine);
    this->regs.eflags = 0x206;

    this->regs.esp = 0xC0000000;

    this->pageDir = reinterpret_cast<PageDirectory*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageDirectory), true));
    memset(reinterpret_cast<uint8_t *>(this->pageDir), 0, sizeof(PageDirectory));

    // Copy mappings, including kernel space... (maybe i should change this??)
    Kernel::MemoryManagement::Paging::CopyDirectory(Kernel::MemoryManagement::Paging::kernelDirectory, this->pageDir);

    /*
    {
        uint32_t start = 0xC0000000;
        uint32_t end = 0xCFFFFF00;
        
        while (start <= end)
        {
            Kernel::MemoryManagement::Paging::FreePage(this->pageDir, start, false);
            start = start + 0x1000;
        }
    }
        */

    // Map user space...
    {
        uint32_t start = 0xC0000000 - 0x4000;
        uint32_t end = 0xC0000000;
        while (start <= end)
        {
            Kernel::MemoryManagement::Paging::AllocatePage(this->pageDir, start, 0, false, true);
            start = start + 0x1000;
        }
    }

    this->regs.cr3 = Kernel::MemoryManagement::Paging::Virtual2Phyiscal(Kernel::MemoryManagement::Paging::kernelDirectory, reinterpret_cast<uint32_t>(this->pageDir));
    this->state = ProcessState_Created;
}

const char* Process::GetName()const
{
    return this->filename;
}

ProcessState& Process::GetState()
{
    return this->state;
}

Context& Process::GetContext()
{
    return this->regs;
}
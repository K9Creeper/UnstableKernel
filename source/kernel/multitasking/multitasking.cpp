/// ----------------
/// multitasking.cpp
/// @brief This file defines the functions and variables that handle multitasking.

#include "multitasking.hpp"

#include "task/task.hpp"
#include "scheduling/scheduling.hpp"

#include "../../chelpers/memory.h"
#include "../../chelpers/string.h"

#include "../memory_management/kheap/kheap.hpp"
#include "../memory_management/paging/paging.hpp"

namespace Kernel
{
    namespace Multitasking
    {
        bool bInitialized = false;
    }
}

void Kernel::Multitasking::Init()
{
    if (bInitialized)
        return;

    Scheduling::Init();

    bInitialized = true;
}

void Kernel::Multitasking::Run()
{
    if (!bInitialized)
        return;

    Scheduling::Run();
}

void Kernel::Multitasking::CreateTask(char *name, void *routine)
{
    uint32_t stackLocation = 0xC0000000;
    Task *t = reinterpret_cast<Task *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(sizeof(Task))));

    memcpy(reinterpret_cast<uint8_t *>(t->name), reinterpret_cast<uint8_t *>(name), strlen(name));

    t->status = TaskStatus_Created;
    t->state.eip = t->routine = routine;
    t->state.eflags = 0x206;
    
    PageDirectory* page = Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageDirectory), true);
    memset(reinterpret_cast<uint8_t*>(page),0, sizeof(PageDirectory));
    Kernel::MemoryManagement::Paging::CopyDirectory(Kernel::MemoryManagement::Paging::kernelDirectory, page);

    t->pageDirectory = page;
    t->cr3 = Kernel::MemoryManagement::Paging::Virtual2Phyiscal(Kernel::MemoryManagement::Paging::kernelDirectory, reinterpret_cast<uint32_t>(page));

    {
        uint32_t start = stackLocation - 0x4000;
        uint32_t end = stackLocation;

        while(start <= end)
        {
            Kernel::MemoryManagement::Paging::AllocatePage(page, start, 0, false, true);
            start = start + 0x1000;
        }
    }

    t->state.esp = stackLocation;

    Scheduling::AddTask(t);

    stackLocation -= 0x4000;
}
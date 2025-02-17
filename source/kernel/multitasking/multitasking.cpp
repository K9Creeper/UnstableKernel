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

void Kernel::Multitasking::CreateTask(const char *name, void *routine)
{
    Task *t = reinterpret_cast<Task *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Task)));
    memset(reinterpret_cast<uint8_t *>(t->name), 0, 512);
    memcpy(reinterpret_cast<uint8_t *>(t->name), reinterpret_cast<const uint8_t *>(name), strlen(name));

    t->status = TaskStatus_Created;
    t->state.eip = routine;
    t->state.eflags = 0x206;
    
    t->pageDirectory = Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(PageDirectory), true);
    memset(reinterpret_cast<uint8_t*>(t->pageDirectory),0, sizeof(PageDirectory));
    Kernel::MemoryManagement::Paging::CopyDirectory(Kernel::MemoryManagement::Paging::kernelDirectory, t->pageDirectory);

    t->cr3 = Kernel::MemoryManagement::Paging::Virtual2Phyiscal(Kernel::MemoryManagement::Paging::kernelDirectory, reinterpret_cast<uint32_t>(t->pageDirectory));

    for(uint32_t start = 0x40000000 - 0x50000; start <= 0x40000000; start+=0x1000)
        Kernel::MemoryManagement::Paging::AllocatePage(t->pageDirectory, start, 0, false, true);

    t->state.esp = t->state.ebp= 0x40000000;

    Scheduling::AddTask(t);
}
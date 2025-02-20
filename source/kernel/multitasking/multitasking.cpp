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
    Task *t = reinterpret_cast<Task *>(Kernel::MemoryManagement::kheap.malloc_(sizeof(Task)));
    memset(reinterpret_cast<uint8_t *>(t->name), 0, 512);
    memcpy(reinterpret_cast<uint8_t *>(t->name), reinterpret_cast<const uint8_t *>(name), strlen(name));

    t->status = TaskStatus_Created;
    t->state.eip = routine;
    t->state.eflags = 0x206;

    //t->heap.PreInit(0x40002000);

    Kernel::MemoryManagement::pManager.AllocateRegion(0x40000000, 0x40500000);

    PageDirectory *pd = Kernel::MemoryManagement::kheap.malloc_(sizeof(PageDirectory), true);
    memset(reinterpret_cast<uint8_t *>(pd), 0, sizeof(PageDirectory));

    Kernel::MemoryManagement::pManager.CopyDirectory(pd);

    t->cr3 = Kernel::MemoryManagement::pManager.Virtual2Phyiscal(reinterpret_cast<uint32_t>(pd));

    t->pManager.Init(reinterpret_cast<uint32_t>(pd), &t->heap);

    //t->heap.Init(0x40400000, 0x40500000, 0x4FFFFF00, false, false, &t->pManager);

    t->pManager.AllocateRegion(0x40000000 - 0x50000, 0x40000000, false, false, true);

    t->state.esp = t->state.ebp = 0x40000000;

    {
        uint32_t i = 0x40000000;
        while (i <= 0x40500000)
        {
            Kernel::MemoryManagement::pManager.FreePage(i, false);
            i += 0x1000;
        }
    }

    Scheduling::AddTask(t);
}
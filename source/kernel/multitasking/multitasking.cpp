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

extern "C" void printf(const char *f, ...);

void Kernel::Multitasking::CreateTask(const char *name, void *routine)
{
    Task *t = reinterpret_cast<Task *>(Kernel::MemoryManagement::kheap.malloc_(sizeof(Task)));
    memset(reinterpret_cast<uint8_t *>(t->name), 0, 512);
    memcpy(reinterpret_cast<uint8_t *>(t->name), reinterpret_cast<const uint8_t *>(name), strlen(name));

    t->status = TaskStatus_Created;
    t->state.eip = routine;
    t->state.eflags = 0x206;

    t->pManager.Init(Kernel::MemoryManagement::kheap.malloc_(sizeof(PageDirectory), true), &Kernel::MemoryManagement::kheap);

    {
        uint32_t i = 0x40000000;
        while (i <= 0x40400000)
        {
            Kernel::MemoryManagement::pManager.AllocatePage(i, 0, false, true, Kernel::MemoryManagement::current);
            i += 0x1000;
        }
    }

    Kernel::MemoryManagement::pManager.CopyDirectory(t->pManager.GetDirectory());

    t->cr3 = Kernel::MemoryManagement::pManager.Virtual2Phyiscal(reinterpret_cast<uint32_t>(t->pManager.GetDirectory()), Kernel::MemoryManagement::current);

    t->state.esp = t->state.ebp = 0x70500000;

    Scheduling::AddTask(t);
}
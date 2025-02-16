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

extern "C" void printf(const char* format, ...);

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

    for(uint32_t start = 0x40000000 - 0x100000; start <= 0x40000000;){
        Kernel::MemoryManagement::Paging::AllocatePage(t->pageDirectory, start, 0, false, true);

        start+=0x1000;
    }

    t->state.esp = t->state.ebp= 0x40000000 - (0x100000/0x2);

    printf(
        "Created Task\n"
        "ds:      0x%X\n"
        "edi:     0x%X\n"
        "esi:     0x%X\n"
        "ebp:     0x%X\n"
        "esp:     0x%X\n"
        "ebx:     0x%X\n"
        "edx:     0x%X\n"
        "ecx:     0x%X\n"
        "eax:     0x%X\n"
        "int_no:  0x%X\n"
        "err_code:0x%X\n"
        "eip:     0x%X\n"
        "cs:      0x%X\n"
        "eflags:  0x%X\n"
        "useresp: 0x%X\n"
        "ss:      0x%X\n",
        t->state.ds, t->state.edi, t->state.esi, t->state.ebp, t->state.esp,
        t->state.ebx, t->state.edx, t->state.ecx, t->state.eax, t->state.int_no,
        t->state.err_code, t->state.eip, t->state.cs, t->state.eflags, t->state.useresp,
        t->state.ss
    );

    Scheduling::AddTask(t);
}
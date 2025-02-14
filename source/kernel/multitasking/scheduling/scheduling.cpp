/// --------------
/// scheduling.cpp
/// @brief This file defines the functions that handle task scheduling.

#include "scheduling.hpp"
#include "scheduling_list/scheduling_list.hpp"

#include "../../memory_management/kheap/kheap.hpp"
#include "../../memory_management/paging/paging.hpp"

#include "../../drivers/pit/pit.hpp"

#include "../../../chelpers/memory.h"

namespace Kernel
{
    namespace Multitasking
    {
        namespace Scheduling
        {
            SchedulingList taskList;

            bool bInitialized = false;

            Task *currentTask = nullptr;
            Task *previousTask = nullptr;
        }
    }
}

extern "C"{
    void switch_stack(Registers* regs);

    void printf(const char* format, ...);
}

void ContextSwitch(Registers* regs, Task *t)
{
    Kernel::Multitasking::Scheduling::currentTask = t;

    if (Kernel::Multitasking::Scheduling::previousTask)
    {
        Kernel::Multitasking::Scheduling::previousTask->status = TaskStatus_Ready;

        memcpy(reinterpret_cast<uint8_t*>(&Kernel::Multitasking::Scheduling::previousTask->state), reinterpret_cast<uint8_t*>(regs), sizeof(Registers));

        asm volatile("mov %%cr3, %0" : "=r"(Kernel::Multitasking::Scheduling::previousTask->cr3));
    }

    if(reinterpret_cast<PageDirectory*>(Kernel::Multitasking::Scheduling::currentTask->cr3)) {
        Kernel::MemoryManagement::Paging::SwitchDirectory(reinterpret_cast<PageDirectory*>(Kernel::Multitasking::Scheduling::currentTask->cr3), true);
    }

    Kernel::Multitasking::Scheduling::currentTask->status = TaskStatus_Running;
    switch_stack(&Kernel::Multitasking::Scheduling::currentTask->state);
}

void Scheduler_(Registers *regs, const uint32_t &tick)
{
    if (Kernel::Multitasking::Scheduling::taskList.GetSize() == 0)
        return;

    Kernel::Multitasking::Scheduling::previousTask = Kernel::Multitasking::Scheduling::currentTask;

    static uint32_t previousIndex = 0;

    if (previousIndex >= Kernel::Multitasking::Scheduling::taskList.GetSize())
        previousIndex = 0;

    Task *next = nullptr;

    for (uint32_t i = previousIndex; i < Kernel::Multitasking::Scheduling::taskList.GetSize();)
    {
        Task *t = Kernel::Multitasking::Scheduling::taskList.Get(i);
        if (!t)
        {
            Kernel::Multitasking::Scheduling::taskList.Remove(i);
            continue;
        }

        if (t == Kernel::Multitasking::Scheduling::currentTask && Kernel::Multitasking::Scheduling::taskList.GetSize() > 1)
        {
            i++;
            continue;
        }

        if(Kernel::Multitasking::Scheduling::previousTask->status == TaskStatus_Zombie)
        {
            // should do a clean up and free

            Kernel::Multitasking::Scheduling::taskList.Remove(i);
            continue;
        }

        next = t;
        previousIndex = i;

        break;
    }

    ContextSwitch(regs, next);
}

void Kernel::Multitasking::Scheduling::Init()
{
    if (bInitialized)
        return;

    taskList.RePlace(reinterpret_cast<void *>(Kernel::MemoryManagement::KHeap::kmalloc_(MAX_TASK_COUNT * sizeof(Task))), MAX_TASK_COUNT);

    bInitialized = true;
}

void Kernel::Multitasking::Scheduling::Run()
{
    if (!bInitialized)
        return;

    Kernel::Drivers::PIT::AddHandle(Scheduler_);
}

void Kernel::Multitasking::Scheduling::AddTask(void *task)
{
    taskList.Insert(task);
}
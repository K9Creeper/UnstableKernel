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
            Task *toDeleteTask = nullptr;
        }
    }
}

extern "C"
{
    void context_switch(Registers *regs);
}

extern "C" void printf(const char *format, ...);

void ContextSwitch(Task *t)
{
    Kernel::Multitasking::Scheduling::currentTask = t;

    if (Kernel::Multitasking::Scheduling::currentTask->pManager->GetDirectory())
    {
        // We wanna use the cr3
        Kernel::Multitasking::Scheduling::currentTask->pManager->SwitchToDirectory(true, reinterpret_cast<PageDirectory *>(Kernel::Multitasking::Scheduling::currentTask->cr3));

        if (Kernel::Multitasking::Scheduling::currentTask->status == TaskStatus_Created)
        {
            if (!Kernel::Multitasking::Scheduling::currentTask->isThread)
                Kernel::Multitasking::Scheduling::currentTask->heap->PreInit(0x40000000);
            
            Kernel::Multitasking::Scheduling::currentTask->pManager->SwapHeap(Kernel::Multitasking::Scheduling::currentTask->heap);

            if (!Kernel::Multitasking::Scheduling::currentTask->isThread)
            {
                Kernel::Multitasking::Scheduling::currentTask->heap->Init(0x40400000, 0x40500000, 0x5FFFFF00, true, false, Kernel::Multitasking::Scheduling::currentTask->pManager);
            }
            Kernel::Multitasking::Scheduling::currentTask->pManager->AllocateRegion(0x70000000, 0x70500000, false, false, true);
        }
    }

    if (Kernel::Multitasking::Scheduling::previousTask && Kernel::Multitasking::Scheduling::previousTask->status == TaskStatus_Zombie)
    {
        Kernel::Multitasking::Scheduling::toDeleteTask = Kernel::Multitasking::Scheduling::previousTask;
    }

    Kernel::Multitasking::Scheduling::previousTask = Kernel::Multitasking::Scheduling::currentTask;

    Kernel::Multitasking::Scheduling::currentTask->status = TaskStatus_Running;
    context_switch(&t->state);

    return;
}

void Scheduler_(Registers *regs, uint32_t tick)
{
    if (Kernel::Multitasking::Scheduling::toDeleteTask)
    {
        if (Kernel::Multitasking::Scheduling::previousTask == Kernel::Multitasking::Scheduling::toDeleteTask)
            Kernel::Multitasking::Scheduling::previousTask = nullptr;

        // We should prolly free memory here!
        // but im actually to lazy to do that...

        Kernel::Multitasking::Scheduling::toDeleteTask = nullptr;
    }

    if (Kernel::Multitasking::Scheduling::taskList.GetSize() == 0)
        return;

    // Like do not look at this...none of this is ideal.....this whole project is not ideal.
    if (Kernel::Multitasking::Scheduling::previousTask && Kernel::Multitasking::Scheduling::previousTask->status != TaskStatus_Zombie && regs)
    {
        Kernel::Multitasking::Scheduling::previousTask->status = TaskStatus_Ready;

        memcpy(reinterpret_cast<uint8_t *>(&Kernel::Multitasking::Scheduling::previousTask->state), reinterpret_cast<uint8_t *>(regs), sizeof(Registers));
        // Lol, what am i doing, i like don't know why, but it just doesnt want to do the thing, jk because this is a userspace task
        Kernel::Multitasking::Scheduling::previousTask->state.esp = Kernel::Multitasking::Scheduling::previousTask->state.useresp;

        asm volatile("mov %%cr3, %0" : "=r"(Kernel::Multitasking::Scheduling::previousTask->cr3));
    }

    static uint32_t prevIdx = 0;
    Task *next = nullptr;

    if (prevIdx >= Kernel::Multitasking::Scheduling::taskList.GetSize() - 1)
        prevIdx = 0;

    for (uint32_t i = prevIdx; i < Kernel::Multitasking::Scheduling::taskList.GetSize(); i++)
    {
        if (prevIdx >= Kernel::Multitasking::Scheduling::taskList.GetSize() - 1)
        {
            prevIdx = 0;
        }

        Task *t = Kernel::Multitasking::Scheduling::taskList.Get(i);
        if (!t || t->status == TaskStatus_Zombie)
        {
            Kernel::Multitasking::Scheduling::taskList.Remove(i);
            if (i - 1 >= 0)
                i--;

            continue;
        }

        if (t == Kernel::Multitasking::Scheduling::currentTask && Kernel::Multitasking::Scheduling::taskList.GetSize() > 1)
        {
            continue;
        }

        next = t;
        prevIdx = i;

        break;
    }

    if (next && next != Kernel::Multitasking::Scheduling::currentTask)
    {
        ContextSwitch(next);
    }
}

void Kernel::Multitasking::Scheduling::Init()
{
    if (bInitialized)
        return;

    taskList.RePlace(reinterpret_cast<void *>(Kernel::MemoryManagement::kheap.malloc_(MAX_TASK_COUNT * sizeof(Task))), MAX_TASK_COUNT);

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
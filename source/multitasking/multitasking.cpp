/// ----------------
/// multitasking.cpp
/// @brief This file defines the functions and variables to handle multitasking in the system.

#include "multitasking.hpp"

#include "wakeups/wakeup.hpp"
#include "wakeups/wakeup_list.hpp"

#include "process/process.hpp"

#include "scheduler/scheduler_list.hpp"

#include "../kernel/drivers/pit/pit.hpp"
#include "../kernel/memory_management/kheap/kheap.hpp"
#include "../kernel/memory_management/paging/paging.hpp"

#include "../chelpers/memory.h"
#include "../chelpers/registers.h"

#define MAX_WAKEUPS_SIZE 256
#define MAX_PROCESSES_SIZE 256

namespace Multitasking
{
    WakeupList wakeupList;

    Registers savedRegisters;

    SchedulerList scheduler;

    Process *currentProcess;
    Process *previousProcess;

    uint32_t previousProcessIndex;

    uint32_t previousTicks;
}

void RegisterWakeup(void *callback, double seconds)
{
    uint32_t ticks = Kernel::Drivers::PIT::ticks + seconds * Kernel::Drivers::PIT::hzFrequency;
    Wakeup *w = reinterpret_cast<Wakeup *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Wakeup)));
    w->callback = callback;
    w->seconds = seconds;
    w->ticks = ticks;

    Multitasking::wakeupList.Insert(w);
}

void WakeupListHandle_(Registers *regs, const uint32_t &ticks)
{
    memcpy(reinterpret_cast<uint8_t *>(&Multitasking::savedRegisters), reinterpret_cast<uint8_t *>(regs), sizeof(Registers));

    for (uint32_t i = 0; i < Multitasking::wakeupList.GetSize(); i++)
    {
        Wakeup *w = reinterpret_cast<Wakeup *>(Multitasking::wakeupList.Get(i));
        reinterpret_cast<wakeup_calback>(w->callback)();
    }
}

extern "C" void printf(const char *format, ...);

extern "C"
{
    void user_regs_switch(Context *regs2);
    void kernel_regs_switch(Context *regs2);
}

void ContextSwitch(Registers *prev, Context *nregs)
{
    printf("Context Switch | Start\n");
    if (Multitasking::previousProcess)
    {
        Context &pregs = Multitasking::previousProcess->GetContext();
        pregs.eax = prev->eax;
        pregs.ebx = prev->ebx;
        pregs.ecx = prev->ecx;
        pregs.edx = prev->edx;
        pregs.esi = prev->esi;
        pregs.edi = prev->edi;
        pregs.ebp = prev->ebp;
        pregs.esp = prev->useresp;
        pregs.eflags = prev->eflags;
        pregs.eip = prev->eip;
        asm volatile("mov %%cr3, %0" : "=r"(pregs.cr3));
    }

    printf("Context Switch | cr3\n");

    if (reinterpret_cast<PageDirectory *>(nregs->cr3) != nullptr)
    {
        Kernel::MemoryManagement::Paging::SwitchDirectory(reinterpret_cast<PageDirectory *>(nregs->cr3), true);
    }

    printf("Context Switch | cr3 done\n");

    printf("Context Switch | outport and swap\n");
    outportb(0x20, 0x20);
    Multitasking::previousProcess = Multitasking::currentProcess;

    printf("Context Switch | user_regs_switch\n");
    user_regs_switch(nregs);
    printf("Context Switch | user_regs_switch done\n");
}

void SchedulerHandle_()
{
    printf("In SchedulerHandle_\n");
    
    if (Multitasking::scheduler.GetSize() == 0)
        return;

    printf("SchedulerHandle_ | Large Enough\n");


    if (!Multitasking::currentProcess)
    {
        printf("SchedulerHandle_ | No Current Process\n");
        Multitasking::previousTicks = Kernel::Drivers::PIT::ticks;
        Multitasking::currentProcess = reinterpret_cast<Process *>(Multitasking::scheduler.Get(0));
        Multitasking::previousProcess = nullptr;
        Multitasking::previousProcessIndex = 0;
        printf("SchedulerHandle_ | Going to ContextSwitch\n");
        ContextSwitch(nullptr, &Multitasking::currentProcess->GetContext());
        printf("SchedulerHandle_ | Done with ContextSwitch\n");
    }

    // reset it
    if (Multitasking::previousProcessIndex >= Multitasking::scheduler.GetSize())
        Multitasking::previousProcessIndex = 0;

    printf("SchedulerHandle_ | Choosing Next\n");

    Process *next = nullptr;
    for (uint32_t i = Multitasking::previousProcessIndex; i < Multitasking::scheduler.GetSize();)
    {
        Process *p = reinterpret_cast<Process *>(Multitasking::scheduler.Get(i));
        if (!p)
        {
            printf("SchedulerHandle_ | Located NULL Process\n");
            Multitasking::scheduler.Remove(i);
            continue;
        }

        if (p == Multitasking::currentProcess && Multitasking::scheduler.GetSize() > 1)
        {
            printf("SchedulerHandle_ | Located Same Process\n");
            i++;
            continue;
        }

        if (Multitasking::previousProcessIndex >= Multitasking::scheduler.GetSize())
        {
            printf("SchedulerHandle_ | Reset Index\n");
            Multitasking::previousProcessIndex = 0;
            continue;
        }

        const ProcessState &state = p->GetState();

        if (state == ProcessState_Zombie)
        {
            printf("SchedulerHandle_ | Found a Zombie\n");
            Multitasking::scheduler.Remove(i);
            continue;
        }

        printf("SchedulerHandle_ | Located Next Process\n");

        next = p;
        Multitasking::previousProcessIndex = i;

        break;
    }

    printf("SchedulerHandle_ | Going to ContextSwitch2\n");
    ContextSwitch(&Multitasking::savedRegisters, &next->GetContext());
    printf("SchedulerHandle_ | Done with ContextSwitch2\n");
}

void Multitasking::Init()
{
    wakeupList.RePlace(reinterpret_cast<void *>(Kernel::MemoryManagement::KHeap::kmalloc_(MAX_WAKEUPS_SIZE * sizeof(Wakeup *))), MAX_WAKEUPS_SIZE);
    scheduler.RePlace(reinterpret_cast<void *>(Kernel::MemoryManagement::KHeap::kmalloc_(MAX_PROCESSES_SIZE * sizeof(Process *))), MAX_PROCESSES_SIZE);

    RegisterWakeup(SchedulerHandle_, 30.0 / Kernel::Drivers::PIT::hzFrequency);

    Kernel::Drivers::PIT::AddHandle(WakeupListHandle_);
}

void Multitasking::CreateProcess(const char *filename, void *routine)
{
    Process *p = reinterpret_cast<Process *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Process)));
    p->Init(filename, routine);

    if(!currentProcess)
        currentProcess = p;
    
    scheduler.Insert(p);
}
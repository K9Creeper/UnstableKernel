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

extern "C" void printf(const char *format, ...);

void WakeupListHandle_(Registers *regs, const uint32_t &ticks)
{
    for (uint32_t i = 0; i < Multitasking::wakeupList.GetSize(); i++)
    {
        Wakeup *w = reinterpret_cast<Wakeup *>(Multitasking::wakeupList.Get(i));
        reinterpret_cast<wakeup_calback>(w->callback)(regs);
    }
}


extern "C"
{
    void user_regs_switch(Context *regs);
    void kernel_regs_switch(Context *regs);
}

void ContextSwitch(Registers *prev, Context *nregs)
{
    if (Multitasking::previousProcess)
    {
        Multitasking::previousProcess->GetState() = ProcessState_Stopped;

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

    Multitasking::previousProcess = Multitasking::currentProcess;

    Multitasking::currentProcess->GetState() = ProcessState_Swapping;

    if (reinterpret_cast<PageDirectory *>(nregs->cr3) != nullptr)
    {
        Kernel::MemoryManagement::Paging::SwitchDirectory(reinterpret_cast<PageDirectory *>(nregs->cr3), true);
    }

    Multitasking::currentProcess->GetState() = ProcessState_Running;

    user_regs_switch(nregs);
}

void SchedulerHandle_(Registers* regs)
{        
    if (Multitasking::scheduler.GetSize() == 0)
        return;
    
    if (!Multitasking::currentProcess)
    {
        Multitasking::previousTicks = Kernel::Drivers::PIT::ticks;
        Multitasking::currentProcess = reinterpret_cast<Process *>(Multitasking::scheduler.Get(0));
        Multitasking::previousProcess = nullptr;
        Multitasking::previousProcessIndex = 0;
        ContextSwitch(nullptr, &Multitasking::currentProcess->GetContext());
    }

    if (Multitasking::previousProcessIndex >= Multitasking::scheduler.GetSize())
        Multitasking::previousProcessIndex = 0;

    if(Multitasking::currentProcess->GetState() == ProcessState_Zombie)
        Multitasking::previousProcess = nullptr;

    Process *next = nullptr;
    for (uint32_t i = Multitasking::previousProcessIndex; i < Multitasking::scheduler.GetSize();)
    {
        Process *p = reinterpret_cast<Process *>(Multitasking::scheduler.Get(i));
        if (!p)
        {
            Multitasking::scheduler.Remove(i);
            continue;
        }

        if (p == Multitasking::currentProcess && Multitasking::scheduler.GetSize() > 1)
        {
            i++;
            continue;
        }

        if (Multitasking::previousProcessIndex >= Multitasking::scheduler.GetSize())
        {
            Multitasking::previousProcessIndex = 0;
            continue;
        }

        const ProcessState &state = p->GetState();

        if (state == ProcessState_Zombie)
        {
            Multitasking::scheduler.Remove(i);
            continue;
        }

        next = p;

        Multitasking::previousProcessIndex = i;

        break;
    }

    Multitasking::currentProcess = next;

    ContextSwitch(regs, &next->GetContext());
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
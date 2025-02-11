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

#include "../chelpers/memory.h"
#include "../chelpers/registers.h"

#define MAX_WAKEUPS_SIZE 256
#define MAX_PROCESSES_SIZE 256

namespace Multitasking{
    WakeupList wakeupList;

    Registers savedRegisters;

    SchedulerList scheduler;
}

void RegisterWakeup(void* callback, double seconds){
    uint32_t ticks = Kernel::Drivers::PIT::ticks + seconds * Kernel::Drivers::PIT::hzFrequency;
    Wakeup* w = reinterpret_cast<Wakeup*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Wakeup)));
    w->callback = callback;
    w->seconds = seconds;
    w->ticks = ticks;

    Multitasking::wakeupList.Insert(w);
}

void WakeupListHandle_(Registers* regs, const uint32_t& ticks)
{
    memcpy(reinterpret_cast<uint8_t*>(&Multitasking::savedRegisters), reinterpret_cast<uint8_t*>(regs), sizeof(Registers));

    for(uint32_t i = 0; i < Multitasking::wakeupList.GetSize(); i++)
    {
        Wakeup* w = reinterpret_cast<Wakeup*>(Multitasking::wakeupList.Get(i));   
        reinterpret_cast<wakeup_calback>(w->callback)();
    }
}

extern "C" void printf(const char* format, ...);

void SchedulerHandle_(){
    if(Multitasking::scheduler.GetSize() == 0)
        return;
    
    
}

void Multitasking::Init(){
    wakeupList.RePlace(reinterpret_cast<void*>(Kernel::MemoryManagement::KHeap::kmalloc_(MAX_WAKEUPS_SIZE * sizeof(Wakeup*))), MAX_WAKEUPS_SIZE);
    scheduler.RePlace(reinterpret_cast<void*>(Kernel::MemoryManagement::KHeap::kmalloc_(MAX_PROCESSES_SIZE * sizeof(Process*))), MAX_PROCESSES_SIZE);

    RegisterWakeup(SchedulerHandle_, 30.0/Kernel::Drivers::PIT::hzFrequency);

    Kernel::Drivers::PIT::AddHandle(WakeupListHandle_);
}

void Multitasking::CreateProcess(const char* filename, void* routine)
{  
    Process* p = reinterpret_cast<Process*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Process)));   
    p->Init(filename, routine);

    scheduler.Insert(p);
}
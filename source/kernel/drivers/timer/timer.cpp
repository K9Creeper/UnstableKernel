/// ---------
/// timer.cpp
/// @brief This file defines the core functions the system 'timer', aswell as it's setups.
/// It also handles a list of 'proccess' that need to be 'woken up'.

#include "timer.hpp"

#include "../../../chelpers/memory.h"

#include "../../memory/interrupt_request/interrupt_request.hpp"
#include "../../memory_management/kheap/kheap.hpp"

#include "timer_array.hpp"

bool TimerArray::lessthan_(void* a, void* b){ return false; }

#define CLOCK_FREQUENCY 1193180

#define ARRAY_MAX_COUNT 256

namespace Kernel{
    namespace Drivers{
        namespace Timer{
            bool bInitialized = false;

            namespace Info{
                uint16_t hz = 0;
                uint32_t jiffies = 0;
            }
            TimerArray wakeupArray;
        }
    }
}

extern "C" void TimerHandle(Registers * reg) {
    Kernel::Drivers::Timer::Info::jiffies++;

    // I should handle the wakeups!
}

void Kernel::Drivers::Timer::Init(uint16_t hz){
    if(bInitialized)
        return;
    
    SetFrequency(hz);
    Kernel::Memory::IRQ::AddHandle(32, TimerHandle);
    //wakeupArray.RePlace(reinterpret_cast<void*>(Kernel::MemoryManagement::KHeap::kmalloc_(ARRAY_MAX_COUNT * sizeof(void*))), ARRAY_MAX_COUNT);

    bInitialized = true;
}

void Kernel::Drivers::Timer::SetFrequency(uint16_t hz) {
    Kernel::Drivers::Timer::Info::hz = hz;
    uint16_t divisor = CLOCK_FREQUENCY / hz;
    
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, (divisor >> 8) & 0xFF);
}
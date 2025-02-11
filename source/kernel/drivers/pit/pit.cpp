/// -------
/// pit.cpp
/// @brief This file defines the functions of the system timer.

#include "pit.hpp"

#include "../../../chelpers/memory.h"

#include "../../memory/interrupt_request/interrupt_request.hpp"

namespace Kernel
{
    namespace Drivers
    {
        namespace PIT
        {
            uint16_t hzFrequency = 0;

            uint32_t ticks = 0;

            void* handles[16];
        }
    }
}

void TimerHandler_(Registers* regs){
    Kernel::Drivers::PIT::ticks++;

    for(uint8_t i = 0; i < 16; i++)
    {
        if(Kernel::Drivers::PIT::handles[i]){
            reinterpret_cast<Kernel::Drivers::PIT::pit_handle>(Kernel::Drivers::PIT::handles[i])(regs, Kernel::Drivers::PIT::ticks);
        }
    }
}

int Kernel::Drivers::PIT::AddHandle(void* handle){
    for(uint8_t i = 0; i < 16; i++)
        if(!handles[i]){
            handles[i] = handle;
            return i;
        }
    return -1;
}

void Kernel::Drivers::PIT::RemoveHandle(int num){
    if(num >= 0 && num < 16)
        handles[num] = nullptr; 
}

void Kernel::Drivers::PIT::Init(uint16_t hz){
    SetFrequency(hz);

    

    Kernel::Memory::IRQ::AddHandle(0, TimerHandler_);
}

void Kernel::Drivers::PIT::SetFrequency(uint16_t hz){
    hzFrequency = hz;
    uint16_t d = 0x1234DC / hz;

    outportb(0x43, 0x36);
    outportb(0x40, d & 0xFF);
    outportb(0x40, (d >> 8) & 0xFF);
}
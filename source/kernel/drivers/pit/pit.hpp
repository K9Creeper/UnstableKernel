/// -------
/// pit.hpp
/// @brief This file declares the structures and functions for the system timer.
#pragma once

#include <stdint.h>

#include "../../../chelpers/registers.h"

namespace Kernel{
    namespace Drivers{
        namespace PIT{
            extern uint16_t hzFrequency;
            extern uint32_t ticks;
            
            typedef void (*pit_handle)(Registers* regs, uint32_t ticks);

            extern int AddHandle(void* handle);
            extern void RemoveHandle(int num);

            extern void Sleep(unsigned int ms);

            extern void Init(uint16_t hz = 100);
            extern void SetFrequency(uint16_t hz);
        }
    }
}

#define sleep Kernel::Drivers::PIT::Sleep
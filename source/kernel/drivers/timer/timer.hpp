/// ---------
/// timer.hpp
/// @brief This file declares the functions for the set up and handling of the system 'timer'

#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Drivers{
        namespace Timer{
            extern bool bInitialized;

            namespace Info{
                extern uint16_t hz;
            }

            extern void Init(uint16_t hz);
            extern void SetFrequency(uint16_t hz);
        }
    }
}
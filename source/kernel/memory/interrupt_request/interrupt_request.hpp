/// ---------------------
/// interrupt_request.hpp
/// @brief This file declares the needed functions to add handles to external (device) interrupts.

#pragma once

#include "../interrupt_service/interrupt_service.hpp"

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace IRQ{
            typedef void(*Handle)(Registers* regs);

            extern void AddHandle(int num, void* handle);
            extern void RemoveHandle(int num);

        }
    }
}

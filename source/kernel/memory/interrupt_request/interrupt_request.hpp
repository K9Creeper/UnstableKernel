#pragma once

#include "../interrupt_service/interrupt_service.hpp"

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace IRQ{
            typedef void(*Handle)(struct Registers regs);

            extern void AddHandle(int num, void* handle);
            extern void RemoveHandle(int num);

        }
    }
}
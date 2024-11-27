#pragma once

#include "../interrupt_service_routines/interrupt_service_routines.hpp"

namespace Kernel
{
    namespace Memory
    {
        namespace IRQ
        {
            typedef void (*fHandle)(Kernel::Memory::ISR::Regs *r);

            extern void Install();

            extern void InstallHandler(int irq, Kernel::Memory::IRQ::fHandle handler);

            extern void UninstallHandler(int irq);
        }
    }
}
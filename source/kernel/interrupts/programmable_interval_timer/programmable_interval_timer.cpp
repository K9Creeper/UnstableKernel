#include "programmable_interval_timer.hpp"

#include "../../memory/interrupt_request/interrupt_request.hpp"

namespace Kernel{
    namespace Interrupts{
        namespace PIT{
            bool bInstalled = false;
        }
    }
}

void Kernel::Interrupts::PIT::Install(void* fHandle){
    if(bInstalled || !fHandle)
        return;
    bInstalled = true;
    Kernel::Memory::IRQ::InstallHandler(0, reinterpret_cast<Kernel::Memory::IRQ::fHandle>(fHandle));
}
#include "keyboard.hpp"

#include "../../../memory/interrupt_request/interrupt_request.hpp"

namespace Kernel
{
    namespace Interrupts
    {
        namespace IO
        {
            namespace Keyboard
            {
                bool bInstalled = false;
            }
        }
    }
}

void Kernel::Interrupts::IO::Keyboard::Install(void* fHandle){
    if(bInstalled || !fHandle)
        return;
    bInstalled = true;
    Kernel::Memory::IRQ::InstallHandler(1, reinterpret_cast<Kernel::Memory::IRQ::fHandle>(fHandle));
}
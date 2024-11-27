#pragma once

namespace Kernel
{
    namespace Interrupts
    {
        namespace IO
        {
            namespace Keyboard
            {
                extern bool bInstalled;

                extern void Install(void* fHandle);                
            }
        }
    }
}
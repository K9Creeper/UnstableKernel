#pragma once

namespace Kernel{
    namespace Interrupts{
        namespace PIT{
            extern bool bInstalled;
            
            extern void Install(void* fHandle);
        }
    }
}
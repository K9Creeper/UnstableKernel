#pragma once

namespace Kernel{
    namespace Debug{
        namespace COM1{
            extern bool bInitialized;

            extern void Init();
        }
    }
}

extern "C" void printf(const char* format, ...);
/// ----------
/// serial.hpp
/// @brief This file declares the needed function(s) to setup the COM1 serial port,
/// aswell as debuging through said port.

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
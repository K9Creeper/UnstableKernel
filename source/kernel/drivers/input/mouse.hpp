/// ---------
/// mouse.hpp
/// @brief This file declares the functions for mouse input.

#pragma once

namespace Kernel{
    namespace Drivers{
        namespace Input{
            namespace Mouse{
                namespace MouseInfo{
                    extern int X;
                    extern int Y;
                }

                extern bool bInitialized;

                extern void Init();
            }
        }
    }
}
/// ---------
/// mouse.hpp
/// @brief This file declares the functions for mouse input.

#pragma once

#include "../../../input/mouse.hpp"

namespace Kernel{
    namespace Drivers{
        namespace Input{
            namespace Mouse{
                extern MouseInfo mouseInfo; 

                extern bool bInitialized;
                
                extern int AddHandle(void *handle);
                extern void RemoveHandle(int i);

                extern void Init();
            }
        }
    }
}
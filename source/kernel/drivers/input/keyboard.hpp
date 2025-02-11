/// ------------
/// keyboard.hpp
/// @brief This file declares the structures, functions and variables to
/// setup the keyboard and get readable input.

#pragma once

#include "../../../input/keyboard.hpp"

namespace Kernel
{
    namespace Drivers
    {
        namespace Input
        {
            namespace Keyboard
            {
                extern bool bInitialized;

                extern void Init();

                extern int AddHandle(void *handle);
                extern void RemoveHandle(int i);

                extern const KeyboardKey *GetKeyMap();
            }
        }
    }
}
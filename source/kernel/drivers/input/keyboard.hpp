/// ------------
/// keyboard.hpp
/// @brief This file declares the structures, functions and variables to
/// setup the keyboard and get readable input.

#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace Drivers
    {
        namespace Input
        {
            namespace Keyboard
            {
                struct Key
                {
                    uint16_t index;

                    char *keyname;

                    bool bhHasValue;
                    char value;

                    bool bPressed;
                    bool bPressedPrev = false;
                };

                typedef void (*keyboard_input_handle)(const Key &k);

                extern bool bInitialized;

                extern void Init();

                extern int AddHandle(void *handle);
                extern void RemoveHandle(int i);

                extern const Key *GetKeyMap();
            }
        }
    }
}
/// ---------
/// input.hpp
/// @brief This file declares the info structures and functions of data related to user input,
// ie. mouse, keyboard, etc...

#pragma once

#include "../../input/mouse.hpp"
#include "../../input/keyboard.hpp"

namespace Usermode{
    namespace Input{
        extern bool bInitialized;

        extern int AddMouseHandle(mouse_input_handle h);
        extern int AddKeyboardHandle(keyboard_input_handle h);

        extern void RemoveMouseHandle(int h);
        extern void RemoveKeyboardHandle(int h);

        extern void Init();
    }
}
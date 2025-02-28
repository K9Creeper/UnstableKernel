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

        extern MouseInfo mouseInfo;
        extern KeyboardKey* keyboardMap;

        // These will be handled in the GUI...
        extern MouseInfo mouseInfoBuffer;
        extern KeyboardKey keyboardMapBuffer;

        extern void Init();
    }
}
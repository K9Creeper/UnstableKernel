/// ------------
/// keyboard.hpp
/// @brief This file declares and defines important strucuters relating to the keyboard.
#pragma once

#include <stdint.h>

struct KeyboardKey
{
    uint16_t index;

    char *keyname;

    bool bhHasValue;
    char value;

    bool bPressed;
    bool bPressedPrev = false;
};

typedef void (*keyboard_input_handle)(const KeyboardKey &k, const KeyboardKey* keymap);
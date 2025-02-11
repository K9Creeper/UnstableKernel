/// ---------
/// mouse.hpp
/// @brief This file declares and defines important strucuters relating to the mouse.
#pragma once

#include <stdint.h>

struct MouseInfo{
    int X;
    int Y;

    int CHANGE_X;
    int CHANGE_Y;

    uint8_t prevState[3];
    uint8_t currState[3];
};

typedef void (*mouse_input_handle)(const MouseInfo &info);
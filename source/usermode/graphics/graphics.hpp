/// ------------
/// graphics.hpp
/// @brief This file declares the functions that handle the OS graphics.

#pragma once

#include <stdint.h>

namespace Usermode{
    namespace Graphics{
        extern bool bThreadRunning;

        extern void Thread();

        extern uint32_t GetScreenX();
        extern uint32_t GetScreenY();

        extern uint32_t GetScreenPitch();
        extern uint32_t GetScreenBPP();
    }
}
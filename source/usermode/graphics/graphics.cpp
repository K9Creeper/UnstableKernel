/// ------------
/// graphics.cpp
/// @brief This file defines the functions that handle the OS graphics.

#include "graphics.hpp"

#include "../../graphics/graphics.hpp"
#include "../../kernel/multitasking/syscall.hpp"

namespace Usermode{
    namespace Graphics{
        bool bThreadRunning = false;

    }
}

void Usermode::Graphics::Thread()
{
    bThreadRunning = true;

    while(bThreadRunning){
        ::Graphics::FillBuffer(0xFF0000);

        

        ::Graphics::SwapBuffers();
    }
    sys_exit();
}
/// ------------
/// usermode.cpp
/// @brief This file defines the usermode entry routine.

#include "../graphics/graphics.hpp"

extern "C" void usermode_main(){
    Graphics::FillBuffer(0x59c2ff);

    Graphics::SwapBuffers();

    for (;;)
    {
        
    }
}
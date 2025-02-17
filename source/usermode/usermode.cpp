/// ------------
/// usermode.cpp
/// @brief This file defines the usermode entry routine.

#include "../graphics/graphics.hpp"

#include "../kernel/multitasking/multitasking.hpp"

#include "../input/mouse.hpp"
#include "../input/keyboard.hpp"

extern "C" void printf(const char* f, ...);

MouseInfo mouseInfo;

void KeyboardHandler(const KeyboardKey &k, const KeyboardKey *keymap)
{

}

void MouseHandler(const MouseInfo &info)
{
   mouseInfo = info;
}

extern "C" void GraphicsThread()
{
   for(;;){
      
      
   }
}
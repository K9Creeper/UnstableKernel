/// ------------
/// usermode.cpp
/// @brief This file defines the usermode entry routine.

#include "../graphics/graphics.hpp"

#include "../kernel/multitasking/multitasking.hpp"

#include "../input/mouse.hpp"
#include "../input/keyboard.hpp"

void KeyboardHandler(const KeyboardKey &k, const KeyboardKey *keymap)
{

}

void MouseHandler(const MouseInfo &info)
{

}

extern "C" void printf(const char* name, ...);

extern "C" void usermode_main()
{
   uint32_t count = 0;
   printf("Hey!\n");

   for(;;){
      
   }
}

extern "C" void usermode_main2()
{
   uint32_t count = 0;
   printf("Hey2!\n");


   for(;;){
      
   }
}
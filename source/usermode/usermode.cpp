/// ------------
/// usermode.cpp
/// @brief This file defines the usermode entry routine.

#include "../kernel/multitasking/syscall.hpp"

#include "graphics/graphics.hpp"

#include "input/input.hpp"

extern "C" void printf(const char* f, ...);

// This function will handle our initialization and idle...
extern "C" void UsermodeEntry()
{
   printf("\n\n| In User Mode |\n\n");

   Usermode::Input::Init();

   sys_create_thread("GraphicsThread", Usermode::Graphics::Thread)

   // WE DO NOT WANT TO EXIT OUTTA THIS GUY
   for(;;);
}

extern "C" void UsermodeSyscallTest(){
   sys_exit();
}
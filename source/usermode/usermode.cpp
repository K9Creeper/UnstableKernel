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
   printf("Input Enabled\n");

   sys_create_thread("GraphicsThread", Usermode::Graphics::Thread)
   printf("Graphics Thread On\n");

   // WE DO NOT WANT TO EXIT OUTTA THIS GUY
   for(;;);
}

extern "C" void UsermodeSyscallTest(){
   sys_exit();
}
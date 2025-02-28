/// ---------
/// input.cpp
/// @brief This file defines the info structures and functions of data related to user input,
// ie. mouse, keyboard, etc...

#include "input.hpp"

#include "../../kernel/multitasking/syscall.hpp"

namespace Usermode{
    namespace Input{
        bool bInitialized = false;

        MouseInfo mouseInfo;

        KeyboardKey* keyboardMap = nullptr;

        MouseInfo mouseInfoBuffer;
        KeyboardKey keyboardMapBuffer;
    }
}


static void MouseHandle(const MouseInfo& mouseInfo){
    Usermode::Input::mouseInfo = mouseInfo;
}

static void KeyboardHandle(const KeyboardKey &k, const KeyboardKey* keymap){
    Usermode::Input::keyboardMap = keymap;
}

void Usermode::Input::Init(){
    if(bInitialized)
        return;

    sys_add_mouse_handle(MouseHandle);
    sys_add_keyboard_handle(KeyboardHandle);

    bInitialized = true;
}
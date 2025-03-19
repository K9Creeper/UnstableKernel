/// ---------
/// input.cpp
/// @brief This file defines the info structures and functions of data related to user input,
// ie. mouse, keyboard, etc...

#include "input.hpp"

#include "../../kernel/multitasking/syscall.hpp"

namespace Usermode
{
    namespace Input
    {
        bool bInitialized = false;

        MouseInfo mouseInfo;

        keyboard_input_handle keyboardHandles[16];
        mouse_input_handle mouseHandles[16];
    }
}

static void MouseHandle(const MouseInfo &mouseInfo)
{
    Usermode::Input::mouseInfo = mouseInfo;

    for (uint8_t i = 0; i < 16; i++)
        if (Usermode::Input::mouseHandles[i])
            Usermode::Input::mouseHandles[i](mouseInfo);
}

static void KeyboardHandle(const KeyboardKey &k, const KeyboardKey *keymap)
{
    for (uint8_t i = 0; i < 16; i++)
        if (Usermode::Input::keyboardHandles[i])
            Usermode::Input::keyboardHandles[i](k, keymap);
}

int Usermode::Input::AddMouseHandle(mouse_input_handle h)
{
    for (uint8_t i = 0; i < 16; i++)
        if (!mouseHandles[i])
        {
            mouseHandles[i] = h;
            return i;
        }
}

int Usermode::Input::AddKeyboardHandle(keyboard_input_handle h)
{
    for (uint8_t i = 0; i < 16; i++)
        if (!keyboardHandles[i])
        {
            keyboardHandles[i] = h;
            return i;
        }
}

void Usermode::Input::RemoveMouseHandle(int h){
    if(h >= 0 && h < 16)
        mouseHandles[h] = nullptr;
}

void Usermode::Input::RemoveKeyboardHandle(int h){
    if(h >= 0 && h < 16)
        keyboardHandles[h] = nullptr;
}

void Usermode::Input::Init()
{
    if (bInitialized)
        return;

    sys_add_mouse_handle(MouseHandle);
    sys_add_keyboard_handle(KeyboardHandle);

    bInitialized = true;
}
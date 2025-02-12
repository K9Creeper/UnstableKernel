/// ------------
/// keyboard.hpp
/// @brief This file defines the functions to setup the keyboard and get readable input.

#include "keyboard.hpp"

#include "../../../chelpers/memory.h"
#include "../../../chelpers/registers.h"

#include "../../memory/interrupt_request/interrupt_request.hpp"

namespace Kernel
{
    namespace Drivers
    {
        namespace Input
        {
            namespace Keyboard
            {
                namespace US_QWETY
                {
                    KeyboardKey keymap[128] = {
                        {0, (char *)"None", false, 0, false},
                        {1, (char *)"Escape", false, 27, false},
                        {2, (char *)"1", true, '1', false},
                        {3, (char *)"2", true, '2', false},
                        {4, (char *)"3", true, '3', false},
                        {5, (char *)"4", true, '4', false},
                        {6, (char *)"5", true, '5', false},
                        {7, (char *)"6", true, '6', false},
                        {8, (char *)"7", true, '7', false},
                        {9, (char *)"8", true, '8', false},
                        {10, (char *)"9", true, '9', false},
                        {11, (char *)"0", true, '0', false},
                        {12, (char *)"Minus", true, '-', false},
                        {13, (char *)"Equal", true, '=', false},
                        {14, (char *)"Backspace", false, '\b', false},
                        {15, (char *)"Tab", false, '\t', false},
                        {16, (char *)"Q", true, 'q', false},
                        {17, (char *)"W", true, 'w', false},
                        {18, (char *)"E", true, 'e', false},
                        {19, (char *)"R", true, 'r', false},
                        {20, (char *)"T", true, 't', false},
                        {21, (char *)"Y", true, 'y', false},
                        {22, (char *)"U", true, 'u', false},
                        {23, (char *)"I", true, 'i', false},
                        {24, (char *)"O", true, 'o', false},
                        {25, (char *)"P", true, 'p', false},
                        {26, (char *)"Left Bracket", true, '[', false},
                        {27, (char *)"Right Bracket", true, ']', false},
                        {28, (char *)"Enter", false, '\n', false},
                        {29, (char *)"Left Control", false, 0, false},
                        {30, (char *)"A", true, 'a', false},
                        {31, (char *)"S", true, 's', false},
                        {32, (char *)"D", true, 'd', false},
                        {33, (char *)"F", true, 'f', false},
                        {34, (char *)"G", true, 'g', false},
                        {35, (char *)"H", true, 'h', false},
                        {36, (char *)"J", true, 'j', false},
                        {37, (char *)"K", true, 'k', false},
                        {38, (char *)"L", true, 'l', false},
                        {39, (char *)"Semicolon", true, ';', false},
                        {40, (char *)"Apostrophe", true, '\'', false},
                        {41, (char *)"Backtick", true, '`', false},
                        {42, (char *)"Left Shift", false, 0, false},
                        {43, (char *)"Backslash", true, '\\', false},
                        {44, (char *)"Z", true, 'z', false},
                        {45, (char *)"X", true, 'x', false},
                        {46, (char *)"C", true, 'c', false},
                        {47, (char *)"V", true, 'v', false},
                        {48, (char *)"B", true, 'b', false},
                        {49, (char *)"N", true, 'n', false},
                        {50, (char *)"M", true, 'm', false},
                        {51, (char *)"Comma", true, ',', false},
                        {52, (char *)"Period", true, '.', false},
                        {53, (char *)"Slash", true, '/', false},
                        {54, (char *)"Right Shift", false, 0, false},
                        {55, (char *)"Multiply", true, '*', false},
                        {56, (char *)"Left Alt", false, 0, false},
                        {57, (char *)"Spacebar", true, ' ', false},
                        {58, (char *)"Caps Lock", false, 0, false},
                        {59, (char *)"F1", false, 0, false},
                        {60, (char *)"F2", false, 0, false},
                        {61, (char *)"F3", false, 0, false},
                        {62, (char *)"F4", false, 0, false},
                        {63, (char *)"F5", false, 0, false},
                        {64, (char *)"F6", false, 0, false},
                        {65, (char *)"F7", false, 0, false},
                        {66, (char *)"F8", false, 0, false},
                        {67, (char *)"F9", false, 0, false},
                        {68, (char *)"F10", false, 0, false},
                        {69, (char *)"Num Lock", false, 0, false},
                        {70, (char *)"Scroll Lock", false, 0, false},
                        {71, (char *)"Home", false, 0, false},
                        {72, (char *)"Up Arrow", false, 0, false},
                        {73, (char *)"Page Up", false, 0, false},
                        {74, (char *)"Num Minus", true, '-', false},
                        {75, (char *)"Left Arrow", false, 0, false},
                        {76, (char *)"Center Arrow", false, 0, false},
                        {77, (char *)"Right Arrow", false, 0, false},
                        {78, (char *)"Num Plus", true, '+', false},
                        {79, (char *)"End", false, 0, false},
                        {80, (char *)"Down Arrow", false, 0, false},
                        {81, (char *)"Page Down", false, 0, false},
                        {82, (char *)"Insert", false, 0, false},
                        {83, (char *)"Delete", false, 0, false},
                        {84, (char *)"Num 0", true, '0', false},
                        {85, (char *)"Num 1", true, '1', false},
                        {86, (char *)"Num 2", true, '2', false},
                        {87, (char *)"Num 3", true, '3', false},
                        {88, (char *)"Num 4", true, '4', false},
                        {89, (char *)"Num 5", true, '5', false},
                        {90, (char *)"Num 6", true, '6', false},
                        {91, (char *)"Num 7", true, '7', false},
                        {92, (char *)"Num 8", true, '8', false},
                        {93, (char *)"Num 9", true, '9', false},
                        {94, (char *)"Num Divide", true, '/', false},
                        {95, (char *)"Num Multiply", true, '*', false},
                        {96, (char *)"Num Subtract", true, '-', false},
                        {97, (char *)"Num Add", true, '+', false},
                        {98, (char *)"Num Enter", true, '\n', false},
                        {99, (char *)"None", false, 0, false},
                        {100, (char *)"None", false, 0, false},
                        {101, (char *)"None", false, 0, false},
                        {102, (char *)"None", false, 0, false},
                        {103, (char *)"None", false, 0, false},
                        {104, (char *)"None", false, 0, false},
                        {105, (char *)"None", false, 0, false},
                        {106, (char *)"None", false, 0, false},
                        {107, (char *)"None", false, 0, false},
                        {108, (char *)"None", false, 0, false},
                        {109, (char *)"None", false, 0, false},
                        {110, (char *)"None", false, 0, false},
                        {111, (char *)"None", false, 0, false},
                        {112, (char *)"None", false, 0, false},
                        {113, (char *)"None", false, 0, false},
                        {114, (char *)"None", false, 0, false},
                        {115, (char *)"None", false, 0, false},
                        {116, (char *)"None", false, 0, false},
                        {117, (char *)"None", false, 0, false},
                        {118, (char *)"None", false, 0, false},
                        {119, (char *)"None", false, 0, false},
                        {120, (char *)"None", false, 0, false},
                        {121, (char *)"None", false, 0, false},
                        {122, (char *)"None", false, 0, false},
                        {123, (char *)"None", false, 0, false},
                        {124, (char *)"None", false, 0, false},
                        {125, (char *)"None", false, 0, false},
                        {126, (char *)"None", false, 0, false},
                        {127, (char *)"None", false, 0, false},
                    };
                }

                void *handles[64];

                bool bInitialized = false;
            }
        }
    }
}

void KeyboardHandler_(Registers *regs)
{
    const uint8_t scancode = inportb(0x60);

    if ((scancode & 128) == 128)
    {
        Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode - 128].bPressedPrev = Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode - 128].bPressed;
        Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode - 128].bPressed = false;
    }
    else
    {
        Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode].bPressedPrev = Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode].bPressed;
        Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode].bPressed = true;
    }

    for (int i = 0; i < 64; i++)
    {
        if (!Kernel::Drivers::Input::Keyboard::handles[i])
            continue;

        if ((scancode & 128) == 128)
        {
            (reinterpret_cast<keyboard_input_handle>(Kernel::Drivers::Input::Keyboard::handles[i]))(Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode - 128], Kernel::Drivers::Input::Keyboard::US_QWETY::keymap);
        }
        else
        {
            (reinterpret_cast<keyboard_input_handle>(Kernel::Drivers::Input::Keyboard::handles[i]))(Kernel::Drivers::Input::Keyboard::US_QWETY::keymap[scancode], Kernel::Drivers::Input::Keyboard::US_QWETY::keymap);
        }
    }
}

extern void Kernel::Drivers::Input::Keyboard::Init()
{
    bInitialized = true;
    Kernel::Memory::IRQ::AddHandle(1, (void *)KeyboardHandler_);
}

int Kernel::Drivers::Input::Keyboard::AddHandle(void *handle)
{
    for (int i = 0; i < 64; i++)
    {
        if (handles[i])
            continue;

        handles[i] = handle;
        return i;
    }

    return -1;
}
void Kernel::Drivers::Input::Keyboard::RemoveHandle(int i)
{
    if (i < 0 || i >= 64)
        return;

    handles[i] = nullptr;
}

const KeyboardKey *Kernel::Drivers::Input::Keyboard::GetKeyMap()
{
    return US_QWETY::keymap;
}
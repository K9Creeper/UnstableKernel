#include "keyboard.hpp"

#include "../../../chelpers/memory.h"
#include "../../memory/interrupt_request/interrupt_request.hpp"

namespace Kernel
{
    namespace Input
    {
        namespace Keyboard
        {
            namespace US_QWETY
            {
                Key keymap[128] = {
                    {0, "None", false, 0, false},
                    {1, "Escape", false, 27, false},
                    {2, "1", true, '1', false},
                    {3, "2", true, '2', false},
                    {4, "3", true, '3', false},
                    {5, "4", true, '4', false},
                    {6, "5", true, '5', false},
                    {7, "6", true, '6', false},
                    {8, "7", true, '7', false},
                    {9, "8", true, '8', false},
                    {10, "9", true, '9', false},
                    {11, "0", true, '0', false},
                    {12, "Minus", true, '-', false},
                    {13, "Equal", true, '=', false},
                    {14, "Backspace", false, '\b', false},
                    {15, "Tab", false, '\t', false},
                    {16, "Q", true, 'q', false},
                    {17, "W", true, 'w', false},
                    {18, "E", true, 'e', false},
                    {19, "R", true, 'r', false},
                    {20, "T", true, 't', false},
                    {21, "Y", true, 'y', false},
                    {22, "U", true, 'u', false},
                    {23, "I", true, 'i', false},
                    {24, "O", true, 'o', false},
                    {25, "P", true, 'p', false},
                    {26, "Left Bracket", true, '[', false},
                    {27, "Right Bracket", true, ']', false},
                    {28, "Enter", false, '\n', false},
                    {29, "Left Control", false, 0, false},
                    {30, "A", true, 'a', false},
                    {31, "S", true, 's', false},
                    {32, "D", true, 'd', false},
                    {33, "F", true, 'f', false},
                    {34, "G", true, 'g', false},
                    {35, "H", true, 'h', false},
                    {36, "J", true, 'j', false},
                    {37, "K", true, 'k', false},
                    {38, "L", true, 'l', false},
                    {39, "Semicolon", true, ';', false},
                    {40, "Apostrophe", true, '\'', false},
                    {41, "Backtick", true, '`', false},
                    {42, "Left Shift", false, 0, false},
                    {43, "Backslash", true, '\\', false},
                    {44, "Z", true, 'z', false},
                    {45, "X", true, 'x', false},
                    {46, "C", true, 'c', false},
                    {47, "V", true, 'v', false},
                    {48, "B", true, 'b', false},
                    {49, "N", true, 'n', false},
                    {50, "M", true, 'm', false},
                    {51, "Comma", true, ',', false},
                    {52, "Period", true, '.', false},
                    {53, "Slash", true, '/', false},
                    {54, "Right Shift", false, 0, false},
                    {55, "Multiply", true, '*', false},
                    {56, "Left Alt", false, 0, false},
                    {57, "Spacebar", true, ' ', false},
                    {58, "Caps Lock", false, 0, false},
                    {59, "F1", false, 0, false},
                    {60, "F2", false, 0, false},
                    {61, "F3", false, 0, false},
                    {62, "F4", false, 0, false},
                    {63, "F5", false, 0, false},
                    {64, "F6", false, 0, false},
                    {65, "F7", false, 0, false},
                    {66, "F8", false, 0, false},
                    {67, "F9", false, 0, false},
                    {68, "F10", false, 0, false},
                    {69, "Num Lock", false, 0, false},
                    {70, "Scroll Lock", false, 0, false},
                    {71, "Home", false, 0, false},
                    {72, "Up Arrow", false, 0, false},
                    {73, "Page Up", false, 0, false},
                    {74, "Num Minus", true, '-', false},
                    {75, "Left Arrow", false, 0, false},
                    {76, "Center Arrow", false, 0, false},
                    {77, "Right Arrow", false, 0, false},
                    {78, "Num Plus", true, '+', false},
                    {79, "End", false, 0, false},
                    {80, "Down Arrow", false, 0, false},
                    {81, "Page Down", false, 0, false},
                    {82, "Insert", false, 0, false},
                    {83, "Delete", false, 0, false},
                    {84, "Num 0", true, '0', false},
                    {85, "Num 1", true, '1', false},
                    {86, "Num 2", true, '2', false},
                    {87, "Num 3", true, '3', false},
                    {88, "Num 4", true, '4', false},
                    {89, "Num 5", true, '5', false},
                    {90, "Num 6", true, '6', false},
                    {91, "Num 7", true, '7', false},
                    {92, "Num 8", true, '8', false},
                    {93, "Num 9", true, '9', false},
                    {94, "Num Divide", true, '/', false},
                    {95, "Num Multiply", true, '*', false},
                    {96, "Num Subtract", true, '-', false},
                    {97, "Num Add", true, '+', false},
                    {98, "Num Enter", true, '\n', false},
                    {99, "None", false, 0, false},
                    {100, "None", false, 0, false},
                    {101, "None", false, 0, false},
                    {102, "None", false, 0, false},
                    {103, "None", false, 0, false},
                    {104, "None", false, 0, false},
                    {105, "None", false, 0, false},
                    {106, "None", false, 0, false},
                    {107, "None", false, 0, false},
                    {108, "None", false, 0, false},
                    {109, "None", false, 0, false},
                    {110, "None", false, 0, false},
                    {111, "None", false, 0, false},
                    {112, "None", false, 0, false},
                    {113, "None", false, 0, false},
                    {114, "None", false, 0, false},
                    {115, "None", false, 0, false},
                    {116, "None", false, 0, false},
                    {117, "None", false, 0, false},
                    {118, "None", false, 0, false},
                    {119, "None", false, 0, false},
                    {120, "None", false, 0, false},
                    {121, "None", false, 0, false},
                    {122, "None", false, 0, false},
                    {123, "None", false, 0, false},
                    {124, "None", false, 0, false},
                    {125, "None", false, 0, false},
                    {126, "None", false, 0, false},
                    {127, "None", false, 0, false},
                };
            }

            void *handles[64];

            bool bInitialized = false;
        }
    }
}

void KeyboardHandler_(struct Kernel_Memory_ISR_Regs *reg)
{
    const uint8_t scancode = inportb(0x60);

    if ((scancode & 128) == 128)
    {
        Kernel::Input::Keyboard::US_QWETY::keymap[scancode-128].bPressedPrev = Kernel::Input::Keyboard::US_QWETY::keymap[scancode-128].bPressed;
        Kernel::Input::Keyboard::US_QWETY::keymap[scancode-128].bPressed = false;
    }
    else
    {
        Kernel::Input::Keyboard::US_QWETY::keymap[scancode].bPressedPrev = Kernel::Input::Keyboard::US_QWETY::keymap[scancode].bPressed;
        Kernel::Input::Keyboard::US_QWETY::keymap[scancode].bPressed = true;
    }

    for (int i = 0; i < 64; i++)
    {
        if (!Kernel::Input::Keyboard::handles[i])
            continue;

        if ((scancode & 128) == 128)
        {
            (reinterpret_cast<Kernel::Input::Keyboard::keyboard_input_handle>(Kernel::Input::Keyboard::handles[i]))(Kernel::Input::Keyboard::US_QWETY::keymap[scancode-128]);
        }
        else
        {
            (reinterpret_cast<Kernel::Input::Keyboard::keyboard_input_handle>(Kernel::Input::Keyboard::handles[i]))(Kernel::Input::Keyboard::US_QWETY::keymap[scancode]);
        }
    }
}

extern void Kernel::Input::Keyboard::Init()
{
    bInitialized = true;
    Kernel::Memory::IRQ::AddHandle(1, (void *)KeyboardHandler_);
}

int Kernel::Input::Keyboard::AddHandle(void *handle)
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
void Kernel::Input::Keyboard::RemoveHandle(int i)
{
    if (i < 0 || i >= 64)
        return;

    handles[i] = nullptr;
}

const Kernel::Input::Keyboard::Key* Kernel::Input::Keyboard::GetKeyMap(){
    return US_QWETY::keymap;
}
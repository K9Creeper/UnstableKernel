#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Terminal{
        extern uint16_t* const buffer;
        
        extern uint16_t vga_width;
        extern uint16_t vga_height;

        namespace Cursor{
            extern unsigned int column;
            extern unsigned int row;
        }

        namespace Customization{
            extern uint16_t color_fg;
            extern uint16_t color_bg;

            extern uint8_t GetColor();
        }

        extern void Clear();

        extern void PutChar(char c);
        extern void RemoveLastChar();

        extern void WriteString(const char* data);
    }
}
#include <stdint.h>

namespace Kernel{
    namespace Terminal{
        extern uint16_t* const buffer;
        
        extern uint16_t vga_width;
        extern uint16_t vga_height;

        extern void Clear();
    }
}
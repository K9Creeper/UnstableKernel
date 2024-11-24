#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace ISR{
            struct Regs
            {
                unsigned int gs, fs, es, ds;      /* pushed the segs last */
                unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
                unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
                unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
            };

            struct Regs16
            {
                uint16_t di;
                uint16_t si;
                uint16_t bp;
                uint16_t sp;
                uint16_t bx;
                uint16_t dx;
                uint16_t cx;
                uint16_t ax;
                // segments
                uint16_t ds;
                uint16_t es;
                uint16_t fs;
                uint16_t gs;
                uint16_t ss;
                uint16_t eflags;
            };

            void Install();
        }
    }
}
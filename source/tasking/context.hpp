/// -------------
/// context.hpp
/// @brief This file only declares the Context structure, which is used to track the 'context' of a proccess.

#pragma once

#include <stdint.h>

namespace Tasking
{
    struct Context
    {
        uint32_t eax;    // 0
        uint32_t ecx;    // 4
        uint32_t edx;    // 8
        uint32_t ebx;    // 12
        uint32_t esp;    // 16
        uint32_t ebp;    // 20
        uint32_t esi;    // 24
        uint32_t edi;    // 28
        uint32_t eflags; // 32
        uint32_t cr3;    // 36
        uint32_t eip;    // 40
    };
}
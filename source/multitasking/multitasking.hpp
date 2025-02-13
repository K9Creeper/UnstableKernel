/// -----------
/// multitasking.hpp
/// @brief This file declares the functions for initating and handling multitasking in the system.

#pragma once

namespace Multitasking{
    extern void Init();
    
    extern void Start();

    extern void CreateProcess(const char* filename, void* routine = nullptr);
}
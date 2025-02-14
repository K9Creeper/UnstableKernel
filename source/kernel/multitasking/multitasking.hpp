/// ----------------
/// multitasking.hpp
/// @brief This file is declares the functions to setting up
/// multitasking in the system.

#pragma once

namespace Kernel
{
    namespace Multitasking
    {
        extern bool bInitialized;

        extern void Init();
        extern void Run();

        extern void CreateTask(char* name, void* routine);
    }
}
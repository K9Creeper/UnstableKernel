/// --------------
/// scheduling.hpp
/// @brief This file declares the functions to handling scheduling.

#pragma once

#define MAX_TASK_COUNT 256

namespace Kernel
{
    namespace Multitasking
    {
        namespace Scheduling
        {
            extern bool bInitialized;

            extern void Init();
            extern void Run();

            extern void AddTask(void* task);
        }
    }
}
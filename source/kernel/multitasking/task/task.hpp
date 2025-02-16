/// --------
/// task.hpp
/// @brief This file defines the Task structure, for keeping track of tasks/processes.

#pragma once

#include <stdint.h>

#include "../../../chelpers/registers.h"

enum TaskStatus : uint16_t {
    TaskStatus_Created = 0,
    TaskStatus_Ready,
    TaskStatus_Zombie,
    TaskStatus_Running
};

struct Task{
    char name[512];
    
    Registers state;

    void* pageDirectory;

    // physical
    uint32_t cr3;

    TaskStatus status;
};
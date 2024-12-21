/// -------------
/// proccess.hpp
/// @brief This file declares the core structure of a proccess, or task. It also declares the function to create a proccess.

#pragma once

#include <stdint.h>

#include "../context.hpp"

#include "../../kernel/memory_management/paging/paging.hpp"

namespace Tasking{
    
    enum State : uint32_t{
        State_RUNNING = 0,
        State_INTERRUPTIBLE,
        State_UNINTERRUPTIBLE,
        State_ZOMBIE = 4,
        State_STOPPED = 8,
        State_SWAPPING = 16,
        State_EXCLUSIVE = 32,
        State_CREATED = 64,
        State_LOADING = 128
    };

    struct Proccess{
        char filename[128];
        uint32_t proccessId;
        
        uint32_t state; // rel to enum State

        uint32_t timeSlice;

        Kernel::MemoryManagement::Paging::PageDirectory* pageDirectory;

        Context context;
    
        void* stack;
    };

    // A Task is the same thing as a Proccess
    typedef Proccess Task;

    extern void CreateProccess(char* filename);
}
/// -----------
/// process.hpp
/// @brief This file declares the process structure, which is a task in the system.

#pragma once

#include "../../memory_management/paging.hpp"

#include "context.hpp"

enum ProcessState : uint16_t{
    ProcessState_Running = 0,
    
    ProcessState_Zombie,
    ProcessState_Stopped,
    ProcessState_Swapping,
    
    ProcessState_Exclusive,
    
    ProcessState_Created,
    ProcessState_Loading,

    ProcessState_Interuptible,
    ProcessState_Uninteruptible,

    ProcessState_NONE
};

struct Process{
private:
    char filename[512];

    ProcessState state = ProcessState_NONE;
    uint32_t time;

protected:
    Context regs;
    uint32_t procId;
    void* stack;

    PageDirectory* pageDir;    
public:
    void Init(char* filename, void * routine = nullptr);

    const char* GetName()const;

    ProcessState& GetState();

    Context& GetContext();
private:

}; 
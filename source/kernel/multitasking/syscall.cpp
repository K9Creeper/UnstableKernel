/// -----------
/// syscall.cpp
/// @brief This file defines the syscall functions.

#include "syscall.hpp"

#include "task/task.hpp"
#include "multitasking.hpp"
#include "scheduling/scheduling.hpp"

#include "../drivers/input/mouse.hpp"
#include "../drivers/input/keyboard.hpp"

namespace Kernel
{
    namespace Multitasking
    {
        namespace Scheduling
        {
            extern Task *currentTask;
        } 
    }
}

extern "C" void printf(const char* format, ...);

extern void Scheduler_(Registers *regs, uint32_t tick);

void Kernel::Multitasking::SYSCALL::_exit(void){
    Scheduling::currentTask->status = TaskStatus_Zombie;


    // So instead of calling the scheduler...
    // basically just waiting till it gets called by the interrupt..
    // this isn't ideal, but I don't really have locks yet (prolly not gonna deal w/ it ngl).

    // Scheduler_(nullptr, 0);

    // Since we waiting, we waiting :]
    for(;;);
}

void Kernel::Multitasking::SYSCALL::_create_thread(const char* name, void* t){
    Kernel::Multitasking::CreateTask(name, t);
}

bool Kernel::Multitasking::SYSCALL::_add_mouse_handle(void* handle){
    return Kernel::Drivers::Input::Mouse::AddHandle(handle) >= 0;
}

bool Kernel::Multitasking::SYSCALL::_add_keyboard_handle(void* handle){
    return Kernel::Drivers::Input::Keyboard::AddHandle(handle) >= 0;
}
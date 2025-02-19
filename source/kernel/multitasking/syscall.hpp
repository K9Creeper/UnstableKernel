/// -----------
/// syscall.hpp
/// @brief This file declares syscall functions.

#pragma once

namespace Kernel{
    namespace Multitasking{
        namespace SYSCALL{            
            extern void _create_thread(const char* name, void* t);

            extern bool _add_mouse_handle(void* handle);
            extern bool _add_keyboard_handle(void* handle);

            extern void _exit(void);
        }
    }
}

#define sys_create_thread(n, t) Kernel::Multitasking::SYSCALL::_create_thread(n, t);

#define sys_add_mouse_handle(h) Kernel::Multitasking::SYSCALL::_add_mouse_handle(h);
#define sys_add_keyboard_handle(h) Kernel::Multitasking::SYSCALL::_add_keyboard_handle(h);

#define sys_exit() Kernel::Multitasking::SYSCALL::_exit();

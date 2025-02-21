/// -----------
/// syscall.hpp
/// @brief This file declares syscall functions.

#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Multitasking{
        namespace SYSCALL{            
            extern void _create_thread(const char* name, void* t);

            extern bool _add_mouse_handle(void* handle);
            extern bool _add_keyboard_handle(void* handle);

            extern void _exit(void);

            extern uint32_t malloc(uint32_t size, bool align = false);
            extern void free(uint32_t loc);
        }
    }
}

#define sys_create_thread Kernel::Multitasking::SYSCALL::_create_thread

#define sys_add_mouse_handle Kernel::Multitasking::SYSCALL::_add_mouse_handle
#define sys_add_keyboard_handle Kernel::Multitasking::SYSCALL::_add_keyboard_handle

#define sys_malloc Kernel::Multitasking::SYSCALL::malloc
#define sys_free Kernel::Multitasking::SYSCALL::free

#define sys_exit Kernel::Multitasking::SYSCALL::_exit

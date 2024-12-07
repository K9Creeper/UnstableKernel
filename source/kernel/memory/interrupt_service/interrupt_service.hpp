/// ---------------------
/// interrupt_service.hpp
/// @brief This file declares the 'Register' structure, aswell as functions
/// relating to the handling of ISRS interrupts.

#pragma once

#include <stdint.h>

struct Registers
{
	uint32_t ds;                                      // Data segment selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha
	uint32_t int_no, err_code;                        // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss;            // Pushed by the processor automatically.
};

namespace Kernel{
	namespace Memory{
		namespace ISRS{
			typedef void(*Handle)(Registers regs);

			extern void AddHandle(uint16_t num, void* handle);
			extern void RemoveHandle(uint16_t num);
		}
	}
}
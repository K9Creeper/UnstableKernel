/// ---------------------
/// interrupt_service.hpp
/// @brief This file declares the 'Register' structure. It does
/// not declare any functions directly related to interrupt_service.cpp

#pragma once

#include <stdint.h>

struct Registers
{
	uint32_t ds;                                      // Data segment selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha
	uint32_t int_no, err_code;                        // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss;            // Pushed by the processor automatically.
};
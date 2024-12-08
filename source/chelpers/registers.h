/// -----------
/// registers.h
/// @brief This file declares the register structures.

#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

struct Registers
{
	uint32_t ds;                                      
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  
	uint32_t int_no, err_code;                        
	uint32_t eip, cs, eflags, useresp, ss;           
};

struct Registers16 {
    uint16_t di;
    uint16_t si;
    uint16_t bp;
    uint16_t sp;
    uint16_t bx;
    uint16_t dx;
    uint16_t cx;
    uint16_t ax;

    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
    uint16_t eflags;
};

#endif
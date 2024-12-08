/// ----------
/// bios32.cpp
/// @brief This file defines the functions to access and call BIOS functions.

#include "bios32.hpp"

#include "../../chelpers/memory.h"

#include "../memory/global_descriptor_table/global_descriptor_table.hpp"
#include "../memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"

#include "../memory_management/paging/paging.hpp"

void (*RebasedBios32Helper)() = reinterpret_cast<void(*)()>(0x7C00);

extern "C" void Bios32Helper();
extern "C" void Bios32HelperEnd();

#define REBASE(x) reinterpret_cast<void*>(0x7c00 + reinterpret_cast<uint32_t>(x) - reinterpret_cast<uint32_t>(Bios32Helper))

extern "C" void * asm_gdt_entries;
extern "C" void * asm_gdt_ptr;
extern "C" void * asm_idt_ptr;
extern "C" void * asm_in_reg_ptr;
extern "C" void * asm_out_reg_ptr;
extern "C" void * asm_intnum_ptr;

Kernel::Memory::IDT::IDTPtr rIDTptr;
Kernel::Memory::GDT::GDTPtr rGDTptr;

extern void GDTSetGate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

extern "C" void printf(const char* format, ...);

void Kernel::Bios32::Init()
{
    // 16 bit code segment
    GDTSetGate(6, 0, 0xFFFFFFFF, 0x9A, 0x0F);
    // 16 bit data segment
    GDTSetGate(7, 0, 0xFFFFFFFF, 0x92, 0x0F);

    rGDTptr.base = reinterpret_cast<uint32_t>(pGDT);
    rGDTptr.limit = sizeof(pGDT) - 1;

    rIDTptr.base = 0;
    rIDTptr.limit = 0x3ff;
}

void Kernel::Bios32::Call(uint8_t int_num, Registers16 * in_reg, Registers16 * out_reg)
{
    void * new_code_base = reinterpret_cast<void*>(0x7C00);
    
    memcpy(reinterpret_cast<unsigned char*>(&asm_gdt_entries), reinterpret_cast<unsigned char*>(pGDT), sizeof(pGDT));

    rGDTptr.base = reinterpret_cast<uint32_t>(REBASE((&asm_gdt_entries)));
    memcpy(reinterpret_cast<unsigned char*>(&asm_gdt_ptr), reinterpret_cast<unsigned char*>(&rGDTptr), sizeof(rGDTptr));

    memcpy(reinterpret_cast<unsigned char*>(&asm_idt_ptr),reinterpret_cast<unsigned char*>(&rIDTptr), sizeof(rIDTptr));

    memcpy(reinterpret_cast<unsigned char*>(&asm_in_reg_ptr), reinterpret_cast<unsigned char*>(in_reg), sizeof(Registers16));
    void * t = REBASE(&asm_in_reg_ptr);
    memcpy(reinterpret_cast<unsigned char*>(&asm_intnum_ptr), reinterpret_cast<unsigned char*>(&int_num), sizeof(uint8_t));

    uint32_t size = reinterpret_cast<uint32_t>(Bios32HelperEnd) - reinterpret_cast<uint32_t>(Bios32Helper);
    memcpy(reinterpret_cast<unsigned char*>(new_code_base), reinterpret_cast<unsigned char*>(Bios32Helper), size);

    RebasedBios32Helper();

    t = REBASE(&asm_out_reg_ptr);
    memcpy(reinterpret_cast<unsigned char*>(out_reg), reinterpret_cast<unsigned char*>(t), sizeof(Registers16));

    Kernel::Memory::GDT::Init();
    Kernel::Memory::GDT::Install();

    Kernel::Memory::IDT::Init();
    Kernel::Memory::IDT::Install();
}
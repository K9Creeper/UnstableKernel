/// ----------------------
/// task_state_segment.cpp
/// @brief This file defines the functions needed to set up the Task State Segment.

#include "task_state_segment.hpp"

#include "../../../chelpers/memory.h"

Kernel::Memory::TSS::TSSEntry tss;

extern void GDTSetGate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

extern "C" void _tss_flush();

void Kernel::Memory::TSS::Init(uint32_t idx, uint32_t kss, uint32_t kesp)
{
    uint32_t base = reinterpret_cast<uint32_t>(&tss);
    GDTSetGate(idx, base, base + sizeof(TSSEntry) - 1, 0xE9, 0x0);
    memset(reinterpret_cast<unsigned char *>(&tss), 0, sizeof(TSSEntry));

    tss.ss0 = kss;

    tss.esp0 = kesp;
    tss.cs = 0x0b;
    tss.ds = 0x13;
    tss.es = 0x13;
    tss.fs = 0x13;
    tss.gs = 0x13;
    tss.ss = 0x13;

    _tss_flush();
}

void Kernel::Memory::TSS::SetStack(uint32_t kss, uint32_t kesp)
{
    tss.ss0 = kss;
    tss.esp0 = kesp;
}
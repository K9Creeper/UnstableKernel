/// --------
/// vesa.cpp
/// @brief This file declares the functions to get and manipulate vesa info.

#include "vesa.hpp"

#include "../../../chelpers/memory.h"

#include "../../bios32/bios32.hpp"
#include "../../memory_management/kheap/kheap.hpp"

namespace Kernel{
    namespace Drivers{
        namespace VESA{
            VbeInfoBlock* vbeInfo = nullptr;
            VbeModeInfoStruct* vbeModeInfo = nullptr;

        }
    }
}

extern "C" void printf(const char* format, ...);

void Kernel::Drivers::VESA::Init(){
    vbeInfo = reinterpret_cast<VbeInfoBlock*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(VbeInfoBlock)));
    vbeModeInfo = reinterpret_cast<VbeModeInfoStruct*>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(VbeModeInfoStruct)));
    
    Registers16 reg_in = {0};
    Registers16 reg_out = {0};

    reg_in.ax = 0x4F00;
    reg_in.di = 0x9500;
    Kernel::Bios32::Call(0x10, &reg_in, &reg_out);
    memcpy(reinterpret_cast<unsigned char*>(vbeInfo), reinterpret_cast<unsigned char*>(0x9500), sizeof(VbeInfoBlock));
    memcpy(reinterpret_cast<unsigned char*>(vbeInfo), reinterpret_cast<unsigned char*>(vbeInfo->VideoModePtr), sizeof(VbeModeInfoStruct));

    printf("Framebuffer: 0x%X\n", vbeModeInfo->framebuffer);
}
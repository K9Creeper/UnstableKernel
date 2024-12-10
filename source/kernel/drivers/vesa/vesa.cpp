/// --------
/// vesa.cpp
/// @brief This file declares the functions to get and manipulate vesa info.

#include "vesa.hpp"

#include "../../../chelpers/memory.h"

#include "../../bios32/bios32.hpp"
#include "../../memory_management/kheap/kheap.hpp"

namespace Kernel
{
    namespace Drivers
    {
        namespace VESA
        {
            VbeInfoBlock *vbeInfo = nullptr;
            VESAMode *vesaModes = nullptr;

            VESAMode currentMode;
        }
    }
}

extern "C" void printf(const char *format, ...);

void VESAGetMode(uint16_t mode, Kernel::Drivers::VESA::VbeModeInfoStruct *modeInfo)
{
    Registers16 reg_in = {0};
    Registers16 reg_out = {0};
    reg_in.ax = 0x4F01;
    reg_in.cx = mode;
    reg_in.di = 0x9000;
    Kernel::Bios32::Call(0x10, &reg_in, &reg_out);
    memcpy(reinterpret_cast<unsigned char *>(modeInfo), reinterpret_cast<unsigned char *>(0x9000), sizeof(Kernel::Drivers::VESA::VbeModeInfoStruct));
}

// make sure to free the return.
Kernel::Drivers::VESA::VESAMode *VESAGetModes()
{
    Kernel::Drivers::VESA::VbeInfoBlock *block = Kernel::Drivers::VESA::vbeInfo;
    Kernel::Drivers::VESA::VbeModeInfoStruct *tmpMode = reinterpret_cast<Kernel::Drivers::VESA::VbeModeInfoStruct *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Kernel::Drivers::VESA::VbeModeInfoStruct)));

    // not sure how many there are, just say 64 for now.
    uint8_t c = 0;
    // ya lets not free this...
    if (!Kernel::Drivers::VESA::vesaModes)
        Kernel::Drivers::VESA::vesaModes = reinterpret_cast<Kernel::Drivers::VESA::VESAMode *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(Kernel::Drivers::VESA::VESAMode) * 64));

    memset(reinterpret_cast<unsigned char *>(Kernel::Drivers::VESA::vesaModes), 0, sizeof(Kernel::Drivers::VESA::VESAMode) * 64);

    uint16_t *list = reinterpret_cast<uint16_t *>(block->videoModes);
    uint16_t mode_number = *list++;
    for (; mode_number != 0xffff && c < 64; mode_number = *list++)
    {
        VESAGetMode(mode_number, tmpMode);

        if ((tmpMode->attributes & 0x90) != 0x90)
            continue;

        if (tmpMode->memoryModel != 4 && tmpMode->memoryModel != 6)
            continue;

        Kernel::Drivers::VESA::vesaModes[c].number = mode_number;
        memcpy(reinterpret_cast<unsigned char *>(&Kernel::Drivers::VESA::vesaModes[c].info), reinterpret_cast<unsigned char *>(tmpMode), sizeof(Kernel::Drivers::VESA::VbeModeInfoStruct));

        c++;
    }
    return Kernel::Drivers::VESA::vesaModes;
}

void VESASetMode(uint32_t mode)
{
    Registers16 reg_in = {0};
    Registers16 reg_out = {0};
    reg_in.ax = 0x4F02;
    reg_in.bx = mode;
    Kernel::Bios32::Call(0x10, &reg_in, &reg_out);
}

void Kernel::Drivers::VESA::SetMode(uint32_t mode){
    VESASetMode(mode);
}

bool Kernel::Drivers::VESA::SetMode(uint32_t width, uint32_t height, uint16_t bpp)
{
    Kernel::Drivers::VESA::VESAMode *m = VESAGetModes();

    for (uint8_t i = 0; i < 64; i++)
    {
        if (m[i].number == 0)
            break;

        if (m[i].info.width != width || m[i].info.height != height || m[i].info.bpp != bpp)
            continue;

        VESASetMode(m[i].number);

        currentMode = m[i];

        return true;
    }
    return false;
}

void Kernel::Drivers::VESA::Init()
{
    vbeInfo = reinterpret_cast<VbeInfoBlock *>(Kernel::MemoryManagement::KHeap::kmalloc_(sizeof(VbeInfoBlock)));

    Registers16 reg_in = {0};
    Registers16 reg_out = {0};

    reg_in.ax = 0x4F00;
    reg_in.di = 0x9500;
    Kernel::Bios32::Call(0x10, &reg_in, &reg_out);
    memcpy(reinterpret_cast<unsigned char *>(vbeInfo), reinterpret_cast<unsigned char *>(0x9500), sizeof(VbeInfoBlock));
}
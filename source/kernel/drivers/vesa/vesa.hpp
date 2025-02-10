/// --------
/// vesa.hpp
/// @brief This file declares the functions to enable vesa, and access vesa information.

#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace Drivers
    {
        namespace VESA
        {
            struct VbeInfoBlock{
                char signature[4];     // must be "VESA" to indicate valid VBE support
                uint16_t version;      // VBE version; high byte is major version, low byte is minor version
                uint32_t oem;          // segment:offset pointer to OEM
                uint32_t capabilities; // bitfield that describes card capabilities
                uint32_t videoModes;   // segment:offset pointer to list of supported video modes
                uint16_t videoMemory;  // amount of video memory in 64KB blocks
                uint16_t softwareRev;  // software revision
                uint32_t vendor;       // segment:offset to card vendor string
                uint32_t productName;  // segment:offset to card model name
                uint32_t productRev;   // segment:offset pointer to product revision
                char reserved[222];    // reserved for future expansion
                char oemData[256];     // OEM BIOSes store their strings in this area
            } __attribute__((packed));
            struct VbeModeInfoStruct{
                uint16_t attributes;
                uint8_t windowA, windowB;
                uint16_t granularity;
                uint16_t windowSize;
                uint16_t segmentA, segmentB;
                uint32_t winFuncPtr;
                uint16_t pitch;

                uint16_t width, height;
                uint8_t wChar, yChar, planes, bpp, banks;
                uint8_t memoryModel, bankSize, imagePages;
                uint8_t reserved0;

                uint8_t readMask, redPosition;
                uint8_t greenMask, greenPosition;
                uint8_t blueMask, bluePosition;
                uint8_t reservedMask, reservedPosition;
                uint8_t directColorAttributes;

                // linear frame buffer
                uint32_t physbase;
                uint32_t offScreenMemOff;
                uint16_t offScreenMemSize;
                uint8_t reserved1[206];
            } __attribute__((packed));

            struct VESAMode{
                uint32_t number;
                VbeModeInfoStruct info;
            };

            extern VbeInfoBlock vbeInfo;
            extern VbeModeInfoStruct *vbeModeInfo;

            extern VESAMode currentMode;

            extern bool bInitalized;

            extern void Init(uint32_t width, uint32_t height);
            extern uint32_t GetLFBAddress();
            extern uint32_t GetMaxLFBAddress();
            extern void SetMode(uint32_t mode);
            extern bool SetMode(uint32_t width, uint32_t height, uint16_t bpp);
        }
    }
}
/// -------
/// pci.hpp
/// @brief This file declares the functions to set up and use PCI(e) devices.

#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace Drivers
    {
        namespace PCI
        {
            union PCIDevice
            {
                uint32_t bits;
                struct
                {
                    uint32_t always_zero : 2;
                    uint32_t field_num : 6;
                    uint32_t function_num : 3;
                    uint32_t device_num : 5;
                    uint32_t bus_num : 8;
                    uint32_t reserved : 7;
                    uint32_t enable : 1;
                };
            };

            enum DeviceType
            {
                PCI_HEADER_TYPE_DEVICE = 0,
                PCI_HEADER_TYPE_BRIDGE,
                PCI_HEADER_TYPE_CARDBUS,
                PCI_TYPE_BRIDGE = 0x0604,
                PCI_TYPE_SATA = 0x0106,
                PCI_NONE = 0xFFFF
            };

            extern void Init();

            extern PCIDevice GetDevice(uint16_t vendor_id, uint16_t device_id, Kernel::Drivers::PCI::DeviceType device_type);
        }
    }
}
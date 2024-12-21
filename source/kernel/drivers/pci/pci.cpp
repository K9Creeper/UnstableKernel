<<<<<<< HEAD
/// -------
/// pci.cpp
/// @brief This file defines the functions to set up and use PCI(e) devices.

#include "pci.hpp"

#include "../../../chelpers/memory.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02
#define PCI_COMMAND 0x04
#define PCI_STATUS 0x06
#define PCI_REVISION_ID 0x08
#define PCI_PROG_IF 0x09
#define PCI_SUBCLASS 0x0a
#define PCI_CLASS 0x0b
#define PCI_CACHE_LINE_SIZE 0x0c
#define PCI_LATENCY_TIMER 0x0d
#define PCI_HEADER_TYPE 0x0e
#define PCI_BIST 0x0f
#define PCI_BAR0 0x10
#define PCI_BAR1 0x14
#define PCI_BAR2 0x18
#define PCI_BAR3 0x1C
#define PCI_BAR4 0x20
#define PCI_BAR5 0x24
#define PCI_INTERRUPT_LINE 0x3C
#define PCI_SECONDARY_BUS 0x09
#define DEVICE_PER_BUS 32
#define FUNCTION_PER_DEVICE 32

uint32_t pci_size_map[100];
Kernel::Drivers::PCI::PCIDevice dev_zero = {0};

void Kernel::Drivers::PCI::Init()
{
    pci_size_map[PCI_VENDOR_ID] = 2;
    pci_size_map[PCI_DEVICE_ID] = 2;
    pci_size_map[PCI_COMMAND] = 2;
    pci_size_map[PCI_STATUS] = 2;
    pci_size_map[PCI_SUBCLASS] = 1;
    pci_size_map[PCI_CLASS] = 1;
    pci_size_map[PCI_CACHE_LINE_SIZE] = 1;
    pci_size_map[PCI_LATENCY_TIMER] = 1;
    pci_size_map[PCI_HEADER_TYPE] = 1;
    pci_size_map[PCI_BIST] = 1;
    pci_size_map[PCI_BAR0] = 4;
    pci_size_map[PCI_BAR1] = 4;
    pci_size_map[PCI_BAR2] = 4;
    pci_size_map[PCI_BAR3] = 4;
    pci_size_map[PCI_BAR4] = 4;
    pci_size_map[PCI_BAR5] = 4;
    pci_size_map[PCI_INTERRUPT_LINE] = 1;
    pci_size_map[PCI_SECONDARY_BUS] = 1;
}

uint32_t PCIRead(Kernel::Drivers::PCI::PCIDevice dev, uint32_t field)
{
    dev.field_num = (field & 0xFC) >> 2;
    dev.enable = 1;
    outportl(PCI_CONFIG_ADDRESS, dev.bits);

    uint32_t size = pci_size_map[field];
    if (size == 1)
    {
        uint8_t t = inportb(PCI_CONFIG_DATA + (field & 3));
        return t;
    }
    else if (size == 2)
    {
        uint16_t t = inports(PCI_CONFIG_DATA + (field & 2));
        return t;
    }
    else if (size == 4)
    {
        uint32_t t = inportl(PCI_CONFIG_DATA);
        return t;
    }
    return 0xffff;
}

void PCIWrite(Kernel::Drivers::PCI::PCIDevice dev, uint32_t field, uint32_t value)
{
    dev.field_num = (field & 0xFC) >> 2;
    dev.enable = 1;

    outportl(PCI_CONFIG_ADDRESS, dev.bits);
    outportl(PCI_CONFIG_DATA, value);
}

uint32_t GetDeviceType(Kernel::Drivers::PCI::PCIDevice dev)
{
    uint32_t t = PCIRead(dev, PCI_CLASS) << 8;
    return t | PCIRead(dev, PCI_SUBCLASS);
}

uint32_t GetSecondaryBus(Kernel::Drivers::PCI::PCIDevice dev)
{
    return PCIRead(dev, PCI_SECONDARY_BUS);
}

uint32_t PCIReachEnd(Kernel::Drivers::PCI::PCIDevice dev)
{
    uint32_t t = PCIRead(dev, PCI_HEADER_TYPE);
    return !t;
}

Kernel::Drivers::PCI::PCIDevice PCIScanFunction(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, uint32_t function, Kernel::Drivers::PCI::DeviceType device_type);
Kernel::Drivers::PCI::PCIDevice PCIScanDevice(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, Kernel::Drivers::PCI::DeviceType device_type);
Kernel::Drivers::PCI::PCIDevice PCIScanBus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, Kernel::Drivers::PCI::DeviceType device_type);
Kernel::Drivers::PCI::PCIDevice Kernel::Drivers::PCI::GetDevice(uint16_t vendor_id, uint16_t device_id, Kernel::Drivers::PCI::DeviceType device_type);

Kernel::Drivers::PCI::PCIDevice PCIScanFunction(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, uint32_t function, Kernel::Drivers::PCI::DeviceType device_type)
{
    Kernel::Drivers::PCI::PCIDevice dev = {0};
    dev.bus_num = bus;
    dev.device_num = device;
    dev.function_num = function;

    if (GetDeviceType(dev) == Kernel::Drivers::PCI::DeviceType::PCI_TYPE_BRIDGE)
    {
        PCIScanBus(vendor_id, device_id, GetSecondaryBus(dev), device_type);
    }

    if (device_type == -1 || device_type == GetDeviceType(dev))
    {
        uint32_t devid = PCIRead(dev, PCI_DEVICE_ID);
        uint32_t vendid = PCIRead(dev, PCI_VENDOR_ID);
        if (devid == device_id && vendor_id == vendid)
            return dev;
    }
    return dev_zero;
}

Kernel::Drivers::PCI::PCIDevice PCIScanDevice(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, Kernel::Drivers::PCI::DeviceType device_type)
{
    Kernel::Drivers::PCI::PCIDevice dev = {0};
    dev.bus_num = bus;
    dev.device_num = device;

    if (PCIRead(dev, PCI_VENDOR_ID) == Kernel::Drivers::PCI::DeviceType::PCI_NONE)
        return dev_zero;

    Kernel::Drivers::PCI::PCIDevice t = PCIScanFunction(vendor_id, device_id, bus, device, 0, device_type);
    if (t.bits)
        return t;

    if (PCIReachEnd(dev))
        return dev_zero;

    for (int function = 1; function < FUNCTION_PER_DEVICE; function++)
    {
        if (PCIRead(dev, PCI_VENDOR_ID) != Kernel::Drivers::PCI::DeviceType::PCI_NONE)
        {
            t = PCIScanFunction(vendor_id, device_id, bus, device, function, device_type);
            if (t.bits)
                return t;
        }
    }
    return dev_zero;
}

Kernel::Drivers::PCI::PCIDevice PCIScanBus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, Kernel::Drivers::PCI::DeviceType device_type)
{
    for (int device = 0; device < DEVICE_PER_BUS; device++)
    {
        Kernel::Drivers::PCI::PCIDevice t = PCIScanDevice(vendor_id, device_id, bus, device, device_type);
        if (t.bits)
            return t;
    }
    return dev_zero;
}

Kernel::Drivers::PCI::PCIDevice Kernel::Drivers::PCI::GetDevice(uint16_t vendor_id, uint16_t device_id, Kernel::Drivers::PCI::DeviceType device_type)
{
    PCIDevice t = PCIScanBus(vendor_id, device_id, 0, device_type);
    if (t.bits)
        return t;

    if (PCIReachEnd(dev_zero))
    {
        // failed
    }
    for (int function = 1; function < FUNCTION_PER_DEVICE; function++)
    {
        Kernel::Drivers::PCI::PCIDevice dev = {0};
        dev.function_num = function;

        if (PCIRead(dev, PCI_VENDOR_ID) == PCI_NONE)
            break;
        t = PCIScanBus(vendor_id, device_id, function, device_type);
        if (t.bits)
            return t;
    }
    return dev_zero;
}
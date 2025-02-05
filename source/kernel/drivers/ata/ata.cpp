/// -------
/// ata.cpp
/// @brief 

#include "ata.hpp"

#include "../pci/pci.hpp"

#include "../../memory/interrupt_request/interrupt_request.hpp"

#include "../../memory_management/kheap/kheap.hpp"

Kernel::Drivers::PCI::PCIDevice ataDevice;

Kernel::Drivers::ATA::ATADevice prMaster = {.slave = 0};
Kernel::Drivers::ATA::ATADevice prSlave = {.slave = 1};
Kernel::Drivers::ATA::ATADevice secMaster = {.slave = 0};
Kernel::Drivers::ATA::ATADevice secSlave = {.slave = 1};

void ATA_HANDLE(Registers* reg){
    inportb(prMaster.status);
    inportb(prMaster.BMR_STATUS);
    outportb(prMaster.BMR_COMMAND, BMR_COMMAND_DMA_STOP);

}

void ATA_DeviceInit(Kernel::Drivers::ATA::ATADevice* device, bool isPrimary){
   
}

void ATA_DetectDevice(Kernel::Drivers::ATA::ATADevice* device, bool isPrimary){

}

void Kernel::Drivers::ATA::Init(){
    ataDevice = Kernel::Drivers::PCI::GetDevice(ATA_VENDOR_ID, ATA_DEVICE_ID, -1);
    
    Kernel::Memory::IRQ::AddHandle(14, ATA_HANDLE);


    
}
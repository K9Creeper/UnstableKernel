#include "interrupt_request.hpp"

#include "../../../c_helpers/memory.h"

extern void IDTSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

extern "C" void _irq0();
extern "C" void _irq1();
extern "C" void _irq2();
extern "C" void _irq3();
extern "C" void _irq4();
extern "C" void _irq5();
extern "C" void _irq6();
extern "C" void _irq7();
extern "C" void _irq8();
extern "C" void _irq9();
extern "C" void _irq10();
extern "C" void _irq11();
extern "C" void _irq12();
extern "C" void _irq13();
extern "C" void _irq14();
extern "C" void _irq15();

void* irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void Kernel::Memory::IRQ::InstallHandler(int irq, Kernel::Memory::IRQ::fHandle handler){
    irq_routines[irq] = reinterpret_cast<void*>(handler);
}

void Kernel::Memory::IRQ::UninstallHandler(int irq){
    irq_routines[irq] = nullptr;
}

void IRQRemap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

extern "C" void _irq_handler(Kernel::Memory::ISR::Regs *r){
    Kernel::Memory::IRQ::fHandle handler;

    if (r->int_no >= 32 && r->int_no <= 47) {
        handler = reinterpret_cast<Kernel::Memory::IRQ::fHandle>(irq_routines[r->int_no - 32]);

        if (handler) {
            handler(r);
        }
    }

    // slave controllers
    if (r->int_no >= 40)
        outportb(0xA0, 0x20);

    // master controllers
    outportb(0x20, 0x20);
}

void Kernel::Memory::IRQ::Install(){
    IRQRemap();

    IDTSetGate(32, reinterpret_cast<unsigned int>(_irq0), 0x08, 0x8E);
    IDTSetGate(33, reinterpret_cast<unsigned int>(_irq1), 0x08, 0x8E);
    IDTSetGate(34, reinterpret_cast<unsigned int>(_irq2), 0x08, 0x8E);
    IDTSetGate(35, reinterpret_cast<unsigned int>(_irq3), 0x08, 0x8E);
    IDTSetGate(36, reinterpret_cast<unsigned int>(_irq4), 0x08, 0x8E);
    IDTSetGate(37, reinterpret_cast<unsigned int>(_irq5), 0x08, 0x8E);
    IDTSetGate(38, reinterpret_cast<unsigned int>(_irq6), 0x08, 0x8E);
    IDTSetGate(39, reinterpret_cast<unsigned int>(_irq7), 0x08, 0x8E);
    IDTSetGate(40, reinterpret_cast<unsigned int>(_irq8), 0x08, 0x8E);
    IDTSetGate(41, reinterpret_cast<unsigned int>(_irq9), 0x08, 0x8E);
    IDTSetGate(42, reinterpret_cast<unsigned int>(_irq10), 0x08, 0x8E);
    IDTSetGate(43, reinterpret_cast<unsigned int>(_irq11), 0x08, 0x8E);
    IDTSetGate(44, reinterpret_cast<unsigned int>(_irq12), 0x08, 0x8E);
    IDTSetGate(45, reinterpret_cast<unsigned int>(_irq13), 0x08, 0x8E);
    IDTSetGate(46, reinterpret_cast<unsigned int>(_irq14), 0x08, 0x8E);
    IDTSetGate(47, reinterpret_cast<unsigned int>(_irq15), 0x08, 0x8E);
}
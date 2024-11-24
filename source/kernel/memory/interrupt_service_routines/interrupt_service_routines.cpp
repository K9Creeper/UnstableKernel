#include "interrupt_service_routines.hpp"

extern void IDTSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

extern "C" void _isr0();
extern "C" void _isr1();
extern "C" void _isr2();
extern "C" void _isr3();
extern "C" void _isr4();
extern "C" void _isr5();
extern "C" void _isr6();
extern "C" void _isr7();
extern "C" void _isr8();
extern "C" void _isr9();
extern "C" void _isr10();
extern "C" void _isr11();
extern "C" void _isr12();
extern "C" void _isr13();
extern "C" void _isr14();
extern "C" void _isr15();
extern "C" void _isr16();
extern "C" void _isr17();
extern "C" void _isr18();
extern "C" void _isr19();
extern "C" void _isr20();
extern "C" void _isr21();
extern "C" void _isr22();
extern "C" void _isr23();
extern "C" void _isr24();
extern "C" void _isr25();
extern "C" void _isr26();
extern "C" void _isr27();
extern "C" void _isr28();
extern "C" void _isr29();
extern "C" void _isr30();
extern "C" void _isr31();

extern "C" void _fault_handler(Kernel::Memory::ISR::Regs *r)
{
    if(r->int_no < 32){
        for(;;);
    }
}

void Kernel::Memory::ISR::Install(){
    IDTSetGate(0, reinterpret_cast<unsigned int>(_isr0), 0x08, 0x8E);
    IDTSetGate(1, reinterpret_cast<unsigned int>(_isr1), 0x08, 0x8E);
    IDTSetGate(2, reinterpret_cast<unsigned int>(_isr2), 0x08, 0x8E);
    IDTSetGate(3, reinterpret_cast<unsigned int>(_isr3), 0x08, 0x8E);
    IDTSetGate(4, reinterpret_cast<unsigned int>(_isr4), 0x08, 0x8E);
    IDTSetGate(5, reinterpret_cast<unsigned int>(_isr5), 0x08, 0x8E);
    IDTSetGate(6, reinterpret_cast<unsigned int>(_isr6), 0x08, 0x8E);
    IDTSetGate(7, reinterpret_cast<unsigned int>(_isr7), 0x08, 0x8E);
    IDTSetGate(8, reinterpret_cast<unsigned int>(_isr8), 0x08, 0x8E);
    IDTSetGate(9, reinterpret_cast<unsigned int>(_isr9), 0x08, 0x8E);
    IDTSetGate(10, reinterpret_cast<unsigned int>(_isr10), 0x08, 0x8E);
    IDTSetGate(11, reinterpret_cast<unsigned int>(_isr11), 0x08, 0x8E);
    IDTSetGate(12, reinterpret_cast<unsigned int>(_isr12), 0x08, 0x8E);
    IDTSetGate(13, reinterpret_cast<unsigned int>(_isr13), 0x08, 0x8E);
    IDTSetGate(14, reinterpret_cast<unsigned int>(_isr14), 0x08, 0x8E);
    IDTSetGate(15, reinterpret_cast<unsigned int>(_isr15), 0x08, 0x8E);
    IDTSetGate(16, reinterpret_cast<unsigned int>(_isr16), 0x08, 0x8E);
    IDTSetGate(17, reinterpret_cast<unsigned int>(_isr17), 0x08, 0x8E);
    IDTSetGate(18, reinterpret_cast<unsigned int>(_isr18), 0x08, 0x8E);
    IDTSetGate(19, reinterpret_cast<unsigned int>(_isr19), 0x08, 0x8E);
    IDTSetGate(20, reinterpret_cast<unsigned int>(_isr20), 0x08, 0x8E);
    IDTSetGate(21, reinterpret_cast<unsigned int>(_isr21), 0x08, 0x8E);
    IDTSetGate(22, reinterpret_cast<unsigned int>(_isr22), 0x08, 0x8E);
    IDTSetGate(23, reinterpret_cast<unsigned int>(_isr23), 0x08, 0x8E);
    IDTSetGate(24, reinterpret_cast<unsigned int>(_isr24), 0x08, 0x8E);
    IDTSetGate(25, reinterpret_cast<unsigned int>(_isr25), 0x08, 0x8E);
    IDTSetGate(26, reinterpret_cast<unsigned int>(_isr26), 0x08, 0x8E);
    IDTSetGate(27, reinterpret_cast<unsigned int>(_isr27), 0x08, 0x8E);
    IDTSetGate(28, reinterpret_cast<unsigned int>(_isr28), 0x08, 0x8E);
    IDTSetGate(29, reinterpret_cast<unsigned int>(_isr29), 0x08, 0x8E);
    IDTSetGate(30, reinterpret_cast<unsigned int>(_isr30), 0x08, 0x8E);
    IDTSetGate(31, reinterpret_cast<unsigned int>(_isr31), 0x08, 0x8E);
}
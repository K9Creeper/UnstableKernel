#include "interrupt_service_routines.h"

#include "../../../c_helpers/string.h"

extern void printf(const char *format, ...);

extern void IDTSetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

void _fault_handler(struct Kernel_Memory_ISR_Regs *r)
{
    if (r->int_no < 32)
    {
        printf("| Fault %d |", r->int_no);
        for (;;)
        {

        }
    }
}

void Kernel_Memory_ISR_Install()
{
    IDTSetGate(0, (unsigned int)(_isr0), 0x08, 0x8E);
    IDTSetGate(1, (unsigned int)(_isr1), 0x08, 0x8E);
    IDTSetGate(2, (unsigned int)(_isr2), 0x08, 0x8E);
    IDTSetGate(3, (unsigned int)(_isr3), 0x08, 0x8E);
    IDTSetGate(4, (unsigned int)(_isr4), 0x08, 0x8E);
    IDTSetGate(5, (unsigned int)(_isr5), 0x08, 0x8E);
    IDTSetGate(6, (unsigned int)(_isr6), 0x08, 0x8E);
    IDTSetGate(7, (unsigned int)(_isr7), 0x08, 0x8E);
    IDTSetGate(8, (unsigned int)(_isr8), 0x08, 0x8E);
    IDTSetGate(9, (unsigned int)(_isr9), 0x08, 0x8E);
    IDTSetGate(10, (unsigned int)(_isr10), 0x08, 0x8E);
    IDTSetGate(11, (unsigned int)(_isr11), 0x08, 0x8E);
    IDTSetGate(12, (unsigned int)(_isr12), 0x08, 0x8E);
    IDTSetGate(13, (unsigned int)(_isr13), 0x08, 0x8E);
    IDTSetGate(14, (unsigned int)(_isr14), 0x08, 0x8E);
    IDTSetGate(15, (unsigned int)(_isr15), 0x08, 0x8E);
    IDTSetGate(16, (unsigned int)(_isr16), 0x08, 0x8E);
    IDTSetGate(17, (unsigned int)(_isr17), 0x08, 0x8E);
    IDTSetGate(18, (unsigned int)(_isr18), 0x08, 0x8E);
    IDTSetGate(19, (unsigned int)(_isr19), 0x08, 0x8E);
    IDTSetGate(20, (unsigned int)(_isr20), 0x08, 0x8E);
    IDTSetGate(21, (unsigned int)(_isr21), 0x08, 0x8E);
    IDTSetGate(22, (unsigned int)(_isr22), 0x08, 0x8E);
    IDTSetGate(23, (unsigned int)(_isr23), 0x08, 0x8E);
    IDTSetGate(24, (unsigned int)(_isr24), 0x08, 0x8E);
    IDTSetGate(25, (unsigned int)(_isr25), 0x08, 0x8E);
    IDTSetGate(26, (unsigned int)(_isr26), 0x08, 0x8E);
    IDTSetGate(27, (unsigned int)(_isr27), 0x08, 0x8E);
    IDTSetGate(28, (unsigned int)(_isr28), 0x08, 0x8E);
    IDTSetGate(29, (unsigned int)(_isr29), 0x08, 0x8E);
    IDTSetGate(30, (unsigned int)(_isr30), 0x08, 0x8E);
    IDTSetGate(31, (unsigned int)(_isr31), 0x08, 0x8E);
}
#include "interrupt_request.h"

#include "../../../c_helpers/memory.h"

extern void IDTSetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

extern void printf(const char *format, ...);

void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

void Kernel_Memory_IRQ_InstallHandler(int irq, void *handler)
{
    irq_routines[irq] = handler;
}

void Kernel_Memory_IRQ_UninstallHandler(int irq)
{
    irq_routines[irq] = 0;
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

extern void _irq_handler(struct Kernel_Memory_ISR_Regs *r)
{
    Kernel_Memory_IRQ_fHandle handler = (Kernel_Memory_IRQ_fHandle)(irq_routines[r->int_no - 32]);
    if (handler)
    {
        handler(r);
    }

    // slave controllers
    if (r->int_no >= 40)
        outportb(0xA0, 0x20);

    // master controllers
    outportb(0x20, 0x20);
}

void Kernel_Memory_IRQ_Install()
{
    IRQRemap();

    IDTSetGate(32, (unsigned)_irq0, 0x08, 0x8E);
    IDTSetGate(33, (unsigned)_irq1, 0x08, 0x8E);
    IDTSetGate(34, (unsigned)_irq2, 0x08, 0x8E);
    IDTSetGate(35, (unsigned)_irq3, 0x08, 0x8E);
    IDTSetGate(36, (unsigned)_irq4, 0x08, 0x8E);
    IDTSetGate(37, (unsigned)_irq5, 0x08, 0x8E);
    IDTSetGate(38, (unsigned)_irq6, 0x08, 0x8E);
    IDTSetGate(39, (unsigned)_irq7, 0x08, 0x8E);
    IDTSetGate(40, (unsigned)_irq8, 0x08, 0x8E);
    IDTSetGate(41, (unsigned)_irq9, 0x08, 0x8E);
    IDTSetGate(42, (unsigned)_irq10, 0x08, 0x8E);
    IDTSetGate(43, (unsigned)_irq11, 0x08, 0x8E);
    IDTSetGate(44, (unsigned)_irq12, 0x08, 0x8E);
    IDTSetGate(45, (unsigned)_irq13, 0x08, 0x8E);
    IDTSetGate(46, (unsigned)_irq14, 0x08, 0x8E);
    IDTSetGate(47, (unsigned)_irq15, 0x08, 0x8E);
}
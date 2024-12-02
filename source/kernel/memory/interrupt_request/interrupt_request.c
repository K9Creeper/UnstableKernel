#include "interrupt_request.h"

#include "../../../c_helpers/memory.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void IDTSetGate(unsigned char num, uint32_t base, unsigned short sel, unsigned char flags);

void *irq_handles[16];

void irq_handler(struct Registers regs)
{
	if (regs.int_no >= 40)
	{
		outportb(0xA0, 0x20);
	}
	outportb(0x20, 0x20);

	// exec
	if (regs.int_no >= 16 && regs.int_no - 32 < 16)
	{
		if (irq_handles[regs.int_no - 32])
		{
			Kernel_Memory_IRQ_Handle handler = (Kernel_Memory_IRQ_Handle)(irq_handles[regs.int_no - 32]);
			if (handler)
			{
				handler(regs);
			}
		}
	}
}

void Kernel_Memory_IRQ_AddHandle(int num, void *handle)
{
	if (num >= 0 && num < 16)
	{
		irq_handles[num] = handle;
	}
}

void Kernel_Memory_IRQ_RemoveHandle(int num)
{
	if (num >= 0 && num < 16)
	{
		irq_handles[num] = 0;
	}
}

void Kernel_Memory_IRQ_Remap()
{
	// Remap the irq table
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x00);
	outportb(0xA1, 0x00);
}

void Kernel_Memory_IRQ_AddGates()
{
	IDTSetGate(32, (uint32_t)irq0, 0x08, 0x8E);
	IDTSetGate(33, (uint32_t)irq1, 0x08, 0x8E);
	IDTSetGate(34, (uint32_t)irq2, 0x08, 0x8E);
	IDTSetGate(35, (uint32_t)irq3, 0x08, 0x8E);
	IDTSetGate(36, (uint32_t)irq4, 0x08, 0x8E);
	IDTSetGate(37, (uint32_t)irq5, 0x08, 0x8E);
	IDTSetGate(38, (uint32_t)irq6, 0x08, 0x8E);
	IDTSetGate(39, (uint32_t)irq7, 0x08, 0x8E);
	IDTSetGate(40, (uint32_t)irq8, 0x08, 0x8E);
	IDTSetGate(41, (uint32_t)irq9, 0x08, 0x8E);
	IDTSetGate(42, (uint32_t)irq10, 0x08, 0x8E);
	IDTSetGate(43, (uint32_t)irq11, 0x08, 0x8E);
	IDTSetGate(44, (uint32_t)irq12, 0x08, 0x8E);
	IDTSetGate(45, (uint32_t)irq13, 0x08, 0x8E);
	IDTSetGate(46, (uint32_t)irq14, 0x08, 0x8E);
	IDTSetGate(47, (uint32_t)irq15, 0x08, 0x8E);
}
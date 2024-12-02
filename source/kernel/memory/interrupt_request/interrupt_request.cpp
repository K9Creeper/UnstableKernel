#include "interrupt_request.hpp"

#include "../../../c_helpers/memory.h"

extern "C"
{
	void irq0();
	void irq1();
	void irq2();
	void irq3();
	void irq4();
	void irq5();
	void irq6();
	void irq7();
	void irq8();
	void irq9();
	void irq10();
	void irq11();
	void irq12();
	void irq13();
	void irq14();
	void irq15();
}

extern void IDTSetGate(unsigned char num, uint32_t base, unsigned short sel, unsigned char flags);

void *irq_handles[16];

extern "C" void irq_handler(Registers regs)
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
			Kernel::Memory::IRQ::Handle handler = reinterpret_cast<Kernel::Memory::IRQ::Handle>(irq_handles[regs.int_no - 32]);
			if (handler)
			{
				handler(regs);
			}
		}
	}
}

void Kernel::Memory::IRQ::AddHandle(int num, void *handle)
{
	if (num >= 0 && num < 16)
	{
		irq_handles[num] = handle;
	}
}

void Kernel::Memory::IRQ::RemoveHandle(int num)
{
	if (num >= 0 && num < 16)
	{
		irq_handles[num] = 0;
	}
}

namespace Kernel
{
	namespace Memory
	{
		namespace IRQ
		{
			void Remap()
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

			void AddGates()
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
		}
	}
}

/// ---------------------
/// interrupt_service.cpp
/// @brief This file defines the core functions for handling and setting up
/// internal interrupts.

#include "interrupt_service.hpp"

extern "C"
{
	void isr0();
	void isr1();
	void isr2();
	void isr3();
	void isr4();
	void isr5();
	void isr6();
	void isr7();
	void isr8();
	void isr9();
	void isr10();
	void isr11();
	void isr12();
	void isr13();
	void isr14();
	void isr15();
	void isr16();
	void isr17();
	void isr18();
	void isr19();
	void isr20();
	void isr21();
	void isr22();
	void isr23();
	void isr24();
	void isr25();
	void isr26();
	void isr27();
	void isr28();
	void isr29();
	void isr30();
	void isr31();
}

extern void IDTSetGate(unsigned char num, uint32_t base, unsigned short sel, unsigned char flags);

extern "C" void printf(const char *format, ...);

void *isrs_handles[32];

extern "C" void isr_handler(struct Registers r)
{
	if (r.int_no < 32)
	{
		printf("| Fault %d |\n", r.int_no);

		if (r.int_no == 14)
		{
			uint32_t faulting_address;

			asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

			// The error code gives us details of what happened
			uint32_t present = r.err_code & 0x1;
			uint32_t rw = r.err_code & 0x2;
			uint32_t user = r.err_code & 0x4;
			uint32_t reserved = r.err_code & 0x8;
			uint32_t inst_fetch = r.err_code & 0x10;

			printf("Possible causes: [ ");
			if (!present)
				printf("Page not present ");
			if (rw)
				printf("Page is read only ");
			if (user)
				printf("Page is read only ");
			if (reserved)
				printf("Overwrote reserved bits ");
			if (inst_fetch)
				printf("Instruction fetch ");
			printf("] at ");
			printf("0x%X\n", faulting_address);
		}

		for (;;)
		{
			asm volatile("hlt");
		}
	}

	Kernel::Memory::ISRS::Handle handler = reinterpret_cast<Kernel::Memory::ISRS::Handle>(isrs_handles[r.int_no]);
	if (handler)
	{
		handler(r);
	}
}

namespace Kernel
{
	namespace Memory
	{
		namespace ISRS
		{
			void AddGates()
			{
				IDTSetGate(0, (uint32_t)isr0, 0x08, 0x8E);
				IDTSetGate(1, (uint32_t)isr1, 0x08, 0x8E);
				IDTSetGate(2, (uint32_t)isr2, 0x08, 0x8E);
				IDTSetGate(3, (uint32_t)isr3, 0x08, 0x8E);
				IDTSetGate(4, (uint32_t)isr4, 0x08, 0x8E);
				IDTSetGate(5, (uint32_t)isr5, 0x08, 0x8E);
				IDTSetGate(6, (uint32_t)isr6, 0x08, 0x8E);
				IDTSetGate(7, (uint32_t)isr7, 0x08, 0x8E);
				IDTSetGate(8, (uint32_t)isr8, 0x08, 0x8E);
				IDTSetGate(9, (uint32_t)isr9, 0x08, 0x8E);
				IDTSetGate(10, (uint32_t)isr10, 0x08, 0x8E);
				IDTSetGate(11, (uint32_t)isr11, 0x08, 0x8E);
				IDTSetGate(12, (uint32_t)isr12, 0x08, 0x8E);
				IDTSetGate(13, (uint32_t)isr13, 0x08, 0x8E);
				IDTSetGate(14, (uint32_t)isr14, 0x08, 0x8E);
				IDTSetGate(15, (uint32_t)isr15, 0x08, 0x8E);
				IDTSetGate(16, (uint32_t)isr16, 0x08, 0x8E);
				IDTSetGate(17, (uint32_t)isr17, 0x08, 0x8E);
				IDTSetGate(18, (uint32_t)isr18, 0x08, 0x8E);
				IDTSetGate(19, (uint32_t)isr19, 0x08, 0x8E);
				IDTSetGate(20, (uint32_t)isr20, 0x08, 0x8E);
				IDTSetGate(21, (uint32_t)isr21, 0x08, 0x8E);
				IDTSetGate(22, (uint32_t)isr22, 0x08, 0x8E);
				IDTSetGate(23, (uint32_t)isr23, 0x08, 0x8E);
				IDTSetGate(24, (uint32_t)isr24, 0x08, 0x8E);
				IDTSetGate(25, (uint32_t)isr25, 0x08, 0x8E);
				IDTSetGate(26, (uint32_t)isr26, 0x08, 0x8E);
				IDTSetGate(27, (uint32_t)isr27, 0x08, 0x8E);
				IDTSetGate(28, (uint32_t)isr28, 0x08, 0x8E);
				IDTSetGate(29, (uint32_t)isr29, 0x08, 0x8E);
				IDTSetGate(30, (uint32_t)isr30, 0x08, 0x8E);
				IDTSetGate(31, (uint32_t)isr31, 0x08, 0x8E);
			}
			void AddHandle(uint16_t num, void *handle)
			{
				isrs_handles[num] = handle;
			}
			void RemoveHandle(uint16_t num)
			{
				isrs_handles[num] = nullptr;
			}
		}
	}
}

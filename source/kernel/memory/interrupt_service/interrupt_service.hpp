/// ---------------------
/// interrupt_service.hpp
/// @brief This file declares the functions relating to the handling of ISRS interrupts.

#pragma once

#include <stdint.h>

#include "../../../chelpers/registers.h"

namespace Kernel{
	namespace Memory{
		namespace ISRS{
			typedef void(*Handle)(Registers regs);

			extern void AddHandle(uint16_t num, void* handle);
			extern void RemoveHandle(uint16_t num);
		}
	}
}
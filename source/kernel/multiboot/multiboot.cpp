#include "multiboot.hpp"

#include "../../chelpers/multiboot.h"

namespace Kernel
{
    namespace Multiboot
    {
        uint32_t mb_info = 0;
        uint32_t mb_magic = 0;
    }
}

uint32_t Kernel::Multiboot::GetPhysicalMemorySize()
{
    
    return 0;
}
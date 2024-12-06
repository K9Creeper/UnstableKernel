#pragma once

#include "../global_descriptor_table/global_descriptor_table.hpp"

namespace Kernel{
    namespace Memory{
        namespace TSS{
            extern void Init(uint32_t idx, uint32_t kss, uint32_t kesp);
            extern void SetStack(uint32_t kss, uint32_t kesp);
        }
    }
}
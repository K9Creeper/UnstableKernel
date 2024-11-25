#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace Paging{
            static uint32_t* pageDirectory = nullptr;
            static uint32_t* lastPage = nullptr;


            extern void Init();
        }
    }
}
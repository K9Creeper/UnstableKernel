#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Memory{
        namespace KHeap{

            namespace heap{
                static uint32_t* last = nullptr;
                static uint32_t* first = nullptr;
            };

            namespace paHeap {
                static uint32_t* last = nullptr;
                static uint32_t* first = nullptr;
                static uint8_t* desc = nullptr;
            }
            
            extern uint32_t lastAlloc;
            extern uint32_t memoryUsed;
            

            extern void Init(uint32_t kernel_end);
            
            extern void Free(void *mem);
            extern unsigned char * Malloc(unsigned int size);

            extern void PAFree(void *mem);
            extern unsigned char * PAMalloc(unsigned int size);
        }
    }
}
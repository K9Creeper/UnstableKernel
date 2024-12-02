#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace Input
    {
        namespace Keyboard
        {
            struct Key
            {
                uint16_t index;

                char* keyname;
                
                bool bhHasValue;
                char value;

                bool bPressed;
            };
            
            typedef void(*keyboard_input_handle)(const Key& k);

            namespace US_QWETY{
                extern Key keymap[128];
            }

            extern void* handles[64];

            extern bool bInitialized;

            extern void Init();
            
            extern int AddHandle(void* handle);
            extern void RemoveHandle(int i);
        }
    }
}
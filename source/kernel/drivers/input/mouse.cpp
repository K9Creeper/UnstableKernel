#include "mouse.hpp"

#include <stdint.h>

#include "../../../chelpers/memory.h"

#include "../../memory/interrupt_request/interrupt_request.hpp"

#define MOUSE_LEFT_BUTTON(flag) (flag & 0x1)
#define MOUSE_RIGHT_BUTTON(flag) (flag & 0x2)
#define MOUSE_MIDDLE_BUTTON(flag) (flag & 0x4)

namespace Kernel{
    namespace Drivers{
        namespace Input{
            namespace Mouse{
                bool bInitialized = false;

                uint8_t cycle = 0;
                char mouse_byte[3];

                uint8_t prevState[3];
                uint8_t currState[3];

                namespace MouseInfo{
                    int X;
                    int Y;
                }

                int CHANGE_X;
                int CHANGE_Y;
            }
        }
    }
}

void Mouse_Wait(uint8_t x) {
    uint32_t timeout=100000;
    if(x==0) {
        while(timeout--)
            if((inportb(0x64) & 1) == 1)
                return;
        return;
    }
    else {
        while(timeout--)
            if(!(inportb(0x64) & 2)) {
                return;
        }
        return;
    }
}

void Mouse_Write(uint8_t x) //unsigned char
{
    Mouse_Wait(1);
    outportb(0x64, 0xD4);
    Mouse_Wait(1);

    outportb(0x60, x);
}

uint8_t Mouse_Read()
{
    Mouse_Wait(0);
    return inportb(0x60);
}

extern "C" void printf(const char* format, ...);

void MouseHandle(Registers * regs){
    if(Kernel::Drivers::Input::Mouse::cycle == 0){
        Kernel::Drivers::Input::Mouse::mouse_byte[0] = Mouse_Read();

        if(MOUSE_LEFT_BUTTON(Kernel::Drivers::Input::Mouse::mouse_byte[0])) {
                Kernel::Drivers::Input::Mouse::currState[0] = 1;
            }
            else {
                Kernel::Drivers::Input::Mouse::currState[0] = 0;
            }

            if(MOUSE_RIGHT_BUTTON(Kernel::Drivers::Input::Mouse::mouse_byte[0])) {
                Kernel::Drivers::Input::Mouse::currState[2] = 1;
            }
            else {
                Kernel::Drivers::Input::Mouse::currState[2] = 0;
            }
            Kernel::Drivers::Input::Mouse::cycle++;
    }else if(Kernel::Drivers::Input::Mouse::cycle == 1){
            Kernel::Drivers::Input::Mouse::mouse_byte[1] = Mouse_Read();
            Kernel::Drivers::Input::Mouse::cycle++;
    }else{
        Kernel::Drivers::Input::Mouse::mouse_byte[2]= Mouse_Read();

        Kernel::Drivers::Input::Mouse::CHANGE_X = Kernel::Drivers::Input::Mouse::mouse_byte[1];
        Kernel::Drivers::Input::Mouse::CHANGE_Y = Kernel::Drivers::Input::Mouse::mouse_byte[2];

        Kernel::Drivers::Input::Mouse::MouseInfo::X += Kernel::Drivers::Input::Mouse::CHANGE_X;
        Kernel::Drivers::Input::Mouse::MouseInfo::Y -= Kernel::Drivers::Input::Mouse::CHANGE_Y;

        if(Kernel::Drivers::Input::Mouse::MouseInfo::X < 0)
                Kernel::Drivers::Input::Mouse::MouseInfo::X = 0;
        if(Kernel::Drivers::Input::Mouse::MouseInfo::Y < 0)
            Kernel::Drivers::Input::Mouse::MouseInfo::Y = 0;

        Kernel::Drivers::Input::Mouse::cycle = 0;
    }

    if(Kernel::Drivers::Input::Mouse::cycle == 0){
        memcpy(Kernel::Drivers::Input::Mouse::prevState, Kernel::Drivers::Input::Mouse::currState, 3);
        memset(Kernel::Drivers::Input::Mouse::currState, 0x00, 3);
    }
}

void Kernel::Drivers::Input::Mouse::Init(){
    bInitialized = true;

    uint8_t temp;  
     
    Mouse_Wait(1);
    outportb(0x64, 0xA8);

    Mouse_Wait(1);
    outportb(0x64, 0x20);
    Mouse_Wait(0);
    temp =(inportb(0x60) | 2);
    Mouse_Wait(1);
    outportb(0x64, 0x60);
    Mouse_Wait(1);
    outportb(0x60, temp);

    Mouse_Write(0xF6);
    Mouse_Read();

    Mouse_Write(0xF4);
    Mouse_Read();

    MouseInfo::X = 20;
    MouseInfo::Y = 20;

    Kernel::Memory::IRQ::AddHandle(12, MouseHandle);
}
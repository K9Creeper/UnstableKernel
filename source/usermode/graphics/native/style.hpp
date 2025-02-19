/// ---------
/// style.hpp
/// @brief This file declares the 'styles' of the native GUI. This includes, mainly things such as color.

#pragma once

#include <stdint.h>

namespace Usermode{
    namespace Graphics{
        namespace Native{
            namespace Style{
                extern uint32_t desktopBkgColor;
                extern uint32_t mouseCursorColor;
                
                extern uint32_t mouseCursorSize;

                extern float desktopNavBarHeightRatio;
                extern uint32_t desktopNavBarColor;
            }
        }
    }
}
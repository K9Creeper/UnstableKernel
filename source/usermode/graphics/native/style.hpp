/// ---------
/// style.hpp
/// @brief This file declares the 'styles' of the native GUI. This includes, mainly things such as color.

#pragma once

#include <stdint.h>

// Current pallet: https://coolors.co/palette/202125-101636-131836-152649-1b345c-1d2a57-a1c5ee-748cca-ebf1f7

namespace Usermode{
    namespace Graphics{
        namespace Native{
            namespace Style{
                extern uint32_t desktopBkgColor;
                extern uint32_t mouseCursorColor;
                
                extern uint32_t mouseCursorSize;

                extern float desktopNavBarHeightRatio;
                extern uint32_t desktopNavBarColor;

                extern uint32_t windowAccent;
                extern float windowTitleBarHeightRatio;
            }
        }
    }
}
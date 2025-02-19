/// ---------
/// style.cpp
/// @brief This file defines the 'styles' of the native GUI. This includes, mainly things such as color.

#include "style.hpp"

// https://coolors.co/palette/131836-154073-2a80b4-f3f6f6-f89641

namespace Usermode{
    namespace Graphics{
        namespace Native{
            namespace Style{
                uint32_t desktopBkgColor = 0x131836;
                uint32_t mouseCursorColor = 0xF89641;
                
                uint32_t mouseCursorSize = 16;

                float desktopNavBarHeightRatio = 0.1;
                uint32_t desktopNavBarColor = 0x154073;

                uint32_t textColor = 0xF3F6F6;
            }
        }
    }
}
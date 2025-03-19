/// ---------
/// style.cpp
/// @brief This file defines the 'styles' of the native GUI. This includes, mainly things such as color.

#include "style.hpp"

namespace Usermode{
    namespace Graphics{
        namespace Native{
            namespace Style{
                uint32_t desktopBkgColor = 0x131836;
                uint32_t mouseCursorColor = 0xA1C5EE;
                
                uint32_t mouseCursorSize = 16;

                float desktopNavBarHeightRatio = 0.08;
                uint32_t desktopNavBarColor = 0x1B345C;
                uint8_t desktopNavBarOpacity = 230;

                uint32_t textColor = 0xEBF1F7;

                uint8_t windowOpacity = 215;
                uint32_t windowAccent = 0x454545;
                float windowTitleBarHeightRatio = 0.08;

                uint32_t windowBorderColor = 0xFFFFFF;
                uint8_t windowBorderThickness = 1;
            }
        }
    }
}
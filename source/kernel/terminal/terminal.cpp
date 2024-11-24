#include "terminal.hpp"

namespace Kernel{
    namespace Terminal{
        uint16_t* const buffer = reinterpret_cast<uint16_t*>(0xB8000);
        uint16_t vga_width = 80;
        uint16_t vga_height = 25;
    }
}

uint8_t Terminal_VGA_Entry_Color(uint16_t fg, uint16_t bg) {
  return fg | bg << 4;
}

uint16_t Terminal_VGA_Entry(unsigned char uc, uint8_t color) {
  return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

void Kernel::Terminal::Clear(){
    for (unsigned int y = 0; y < Kernel::Terminal::vga_height; y++) {
    for (unsigned int x = 0; x < Kernel::Terminal::vga_width; x++) {
      const unsigned int index = y * Kernel::Terminal::vga_width + x;
      Kernel::Terminal::buffer[index] = Terminal_VGA_Entry(' ', Terminal_VGA_Entry_Color(2, 0));
    }
  }
}
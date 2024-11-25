#include "terminal.hpp"

#include "../../c_helpers/memory.h"
#include "../../c_helpers/string.h"

///
/// Defs..
///

namespace Kernel
{
  namespace Terminal
  {
    uint16_t *const buffer = reinterpret_cast<uint16_t *>(0xB8000);

    uint16_t vga_width = 80;
    uint16_t vga_height = 25;

    namespace Cursor
    {
      unsigned int row = 0;
      unsigned int column = 0;
    }

    namespace Customization
    {
      uint16_t color_fg = 2;
      uint16_t color_bg = 0;
    }
  }
}

///
/// Helpers
///

uint8_t Terminal_VGA_Entry_Color(uint16_t fg, uint16_t bg)
{
  return fg | bg << 4;
}

uint16_t Terminal_VGA_Entry(unsigned char uc, uint8_t color)
{
  return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

void Terminal_Enable_Cursor()
{
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, (inportb(0x3D5) & 0xC0) | 0);

	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3D5) & 0xE0) | 237);
}

void Terminal_Disable_Cursor()
{
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}

void Terminal_Cursor_Update(uint16_t x, uint16_t y)
{
  uint16_t pos = y * Kernel::Terminal::vga_width + x;
  outportb(0x3D4, 0x0F);
  outportb(0x3D5, static_cast<uint8_t>(pos & 0xFF));
  outportb(0x3D4, 0x0E);
  outportb(0x3D5, static_cast<uint8_t>((pos >> 8) & 0xFF));
}

void Terminal_Put_Entry_At(char c, uint8_t color, unsigned int x, unsigned int y)
{
  const unsigned int index = y * Kernel::Terminal::vga_width + x;
  Kernel::Terminal::buffer[index] = Terminal_VGA_Entry(c, color);
}

void Terminal_Write(const char *data, unsigned int size)
{
  for (unsigned int i = 0; i < size; i++)
  {
    Kernel::Terminal::PutChar(data[i]);
  }
  Terminal_Cursor_Update(Kernel::Terminal::Cursor::column, Kernel::Terminal::Cursor::row);
}

///
/// Function Defs...
///
uint8_t Kernel::Terminal::Customization::GetColor()
{
  return Terminal_VGA_Entry_Color(Kernel::Terminal::Customization::color_fg, Kernel::Terminal::Customization::color_bg);
}

void Kernel::Terminal::Init(){
  Terminal_Enable_Cursor();
  Kernel::Terminal::Clear();
}

void Kernel::Terminal::Clear()
{
  for (unsigned int y = 0; y < Kernel::Terminal::vga_height; y++)
  {
    for (unsigned int x = 0; x < Kernel::Terminal::vga_width; x++)
    {
      const unsigned int index = y * Kernel::Terminal::vga_width + x;
      Kernel::Terminal::buffer[index] = Terminal_VGA_Entry(' ', Kernel::Terminal::Customization::GetColor());
    }
  }

  Kernel::Terminal::Cursor::column = 0;
  Kernel::Terminal::Cursor::row = 0;

  Terminal_Cursor_Update(0, 0);
}

void Kernel::Terminal::PutChar(char c)
{
  if (c == '\n')
  {
    Kernel::Terminal::Cursor::column = 0;
    Kernel::Terminal::Cursor::row++;
    return;
  }
  Terminal_Put_Entry_At(c, Kernel::Terminal::Customization::GetColor(), Kernel::Terminal::Cursor::column, Kernel::Terminal::Cursor::row);
  if (++Kernel::Terminal::Cursor::column == Kernel::Terminal::vga_width)
  {
    Kernel::Terminal::Cursor::column = 0;
    if (++Kernel::Terminal::Cursor::row == Kernel::Terminal::vga_height)
      Kernel::Terminal::Cursor::row = 0;
  }
}

void Kernel::Terminal::RemoveLastChar()
{
  Terminal_Put_Entry_At(' ', 0, Kernel::Terminal::Cursor::column, Kernel::Terminal::Cursor::row);
  if (--Kernel::Terminal::Cursor::column == Kernel::Terminal::vga_width)
  {
    Kernel::Terminal::Cursor::column = 0;
    if (Kernel::Terminal::Cursor::row == Kernel::Terminal::vga_height)
      Kernel::Terminal::Cursor::row = 0;
  }
  Terminal_Cursor_Update(Kernel::Terminal::Cursor::column, Kernel::Terminal::Cursor::row);
}

void Kernel::Terminal::WriteString(const char *data)
{
  Terminal_Write(data, strlen(data));
}
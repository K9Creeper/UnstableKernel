#include "task_state_segment.hpp"

#include "../../../c_helpers/memory.h"

#include "../global_descriptor_table/global_descriptor_table.hpp"

namespace Kernel{
    namespace Memory {
        namespace TSS{
            
        }
    }
}


extern "C" uint32_t _get_eip();
extern "C" void _flush_tss();
extern "C" void _gdt_flush();

extern void GDTSetGate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

void ZeroTSS(uint32_t limit){
  memset(reinterpret_cast<unsigned char*>(&Kernel::Memory::TSS::entry), 0, limit);
}

void TSS_Write(int index, uint16_t ss0, uint32_t esp0){
  /* https://wiki.osdev.org/Getting_to_Ring_3 */
  uint32_t eip = _get_eip();
  uint32_t base = reinterpret_cast<uint32_t>(&Kernel::Memory::TSS::entry);
  uint32_t limit = base + sizeof(Kernel::Memory::TSS::entry);

  GDTSetGate(index, reinterpret_cast<uint32_t>(&Kernel::Memory::TSS::entry), limit, 0x89, 0x0);

  ZeroTSS(sizeof(Kernel::Memory::TSS::entry));

      Kernel::Memory::TSS::entry.ss0 = ss0;
      Kernel::Memory::TSS::entry.esp0 = esp0;
      Kernel::Memory::TSS::entry.eip = eip;
      Kernel::Memory::TSS::entry.cs = 0x0b;
      Kernel::Memory::TSS::entry.ss = 0x13;
      Kernel::Memory::TSS::entry.ds = 0x13;
      Kernel::Memory::TSS::entry.es = 0x13;
      Kernel::Memory::TSS::entry.fs = 0x13;
      Kernel::Memory::TSS::entry.gs = 0x13;
      Kernel::Memory::TSS::entry.iomap_base = sizeof(Kernel::Memory::TSS::TSSEntry);
}

void TSS_Set_Kernel_Stack(uint32_t stack) { 
  // Used when an interrupt occurs
  Kernel::Memory::TSS::entry.esp0 = stack;
}

void Kernel::Memory::TSS::Install(){
  TSS_Write(5, 0x10, 0x00100000);

  _flush_tss();

  _gdt_flush();
}
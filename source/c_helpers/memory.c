#include "memory.h"

char inportb(unsigned short _port) {
  unsigned char rv;
  asm volatile("inb %1, %0": "=a"(rv): "dN"(_port));
  return rv;
}

void outportb(unsigned short _port, unsigned char _data) {
  asm volatile("outb %1, %0":: "dN"(_port), "a"(_data));
}

void call_asm(void* fn, void* arg)
{
    asm volatile(
    "movl  %[FUNC], %%eax\n"
    "movl %[ARG], %%edi\n"
    "call *%%eax"
    : : [FUNC]"a"((int)fn), [ARG]"b"((int)arg)
      );
}

void swap(int *p1, int *p2) {
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

unsigned char * memcpy(unsigned char * dest,
  const unsigned char * src, int count) {
  unsigned char * csrc = (unsigned char * ) src;
  unsigned char * cdest = (unsigned char * ) dest;

  // Copy contents of src[] to dest[]  
  for (int i = 0; i < count; i++)
    cdest[i] = csrc[i];
  return cdest;
}

unsigned char * memset(unsigned char * dest, unsigned char val, int count) {
  register unsigned char * ptr = (unsigned char * ) dest;
  while (count-- > 0)
    *ptr++ = val;
  return dest;
}

unsigned short * memsetw(unsigned short * dest, unsigned short val, int count) {
  register unsigned short * ptr = (unsigned short * ) dest;
  while (count-- > 0)
    *ptr++ = val;
  return dest;
}
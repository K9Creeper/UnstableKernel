#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#ifdef __cplusplus

extern "C"
{
    uint8_t inportb(unsigned short _port);

    void outportb(unsigned short _port, uint8_t _data);

    uint16_t inports(unsigned short _port);

    void outports(unsigned short _port, uint16_t _data);

    uint32_t inportl(unsigned short _port);

    void outportl(unsigned short _port, uint16_t _data);

    void call_asm(void *fn, void *arg);

    void swap(int *p1, int *p2);

    unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);

    unsigned char *memset(unsigned char *dest, unsigned char val, int count);

    unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
}

#else

uint8_t inportb(unsigned short _port);

extern void outportb(unsigned short _port, uint8_t _data);

extern uint16_t inports(unsigned short _port);

extern void outports(unsigned short _port, uint16_t _data);

extern uint32_t inportl(unsigned short _port);

extern void outportl(unsigned short _port, uint16_t _data);

extern void call_asm(void *fn, void *arg);

extern void swap(int *p1, int *p2);

extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);

extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);

extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);

#endif

#endif // MEMORY_H
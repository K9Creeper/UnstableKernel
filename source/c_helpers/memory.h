#ifndef MEMORY_H
#define MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

    char inportb(unsigned short _port);

    void outportb(unsigned short _port, unsigned char _data);

    void call_asm(void* fn, void* arg);

    void swap(int *p1, int *p2);

    unsigned char * memcpy(unsigned char * dest, const unsigned char * src, int count);

    unsigned char * memset(unsigned char * dest, unsigned char val, int count);

    unsigned short * memsetw(unsigned short * dest, unsigned short val, int count);

#ifdef __cplusplus
}
#endif

#endif // MEMORY_H
#ifndef MEMORY_H
#define MEMORY_H

#ifdef __cplusplus
extern "C" {

    char inportb(unsigned short _port);

    void outportb(unsigned short _port, unsigned char _data);

    void call_asm(void* fn, void* arg);

    void swap(int *p1, int *p2);

    unsigned char * memcpy(unsigned char * dest, const unsigned char * src, int count);

    unsigned char * memset(unsigned char * dest, unsigned char val, int count);

    unsigned short * memsetw(unsigned short * dest, unsigned short val, int count);


}

#else

    extern char inportb(unsigned short _port);

    extern void outportb(unsigned short _port, unsigned char _data);

    extern void call_asm(void* fn, void* arg);

    extern void swap(int *p1, int *p2);

    extern unsigned char * memcpy(unsigned char * dest, const unsigned char * src, int count);

    extern unsigned char * memset(unsigned char * dest, unsigned char val, int count);

    extern unsigned short * memsetw(unsigned short * dest, unsigned short val, int count);

#endif

#endif // MEMORY_H
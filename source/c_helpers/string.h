#ifndef STRING_H
#define STRING_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool isletter(const char c);
    bool islower(const char c);
    bool isdigit(const char c);
    bool ispunct(const char c);

    int strlen(const char *str);

    int find(const char *str, const char *f);
    void itoa(int num, char *str, int base);

#ifdef __cplusplus
}
#endif

#endif
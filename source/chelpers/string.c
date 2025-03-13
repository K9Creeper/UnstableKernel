#include "string.h"

#include <stdint.h>

bool isletter(const char c) { return (c >= 'A' && c <= 'z'); }
bool islower(const char c) { return (c >= 'a' && c <= 'z'); }
bool isdigit(const char c) { return (c >= '0' && c <= '9'); }
bool ispunct(const char c)
{
  return (c == '!' || c == '"' || c == '#' || c == '$' || c == '%' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == ':' || c == ';' || c == '?' || c == '@' || c == '[' || c == '\\' || c == ']' || c == '^' || c == '_' || c == '`' || c == '{' || c == '|' || c == '}' || c == '~');
}

int strlen(const char *str)
{
  uint32_t len = 0;
  while (str[len])
    len++;
  return len;
}

char *strsep(char **stringp, const char *delim)
{
  char *s;
  const char *spanp;
  int c, sc;
  char *tok;
  if ((s = *stringp) == 0)
      return 0;
  for (tok = s;;) {
      c = *s++;
      spanp = delim;
      do {
          if ((sc = *spanp++) == c) {
              if (c == 0)
                  s = 0;
              else
                  s[-1] = 0;
              *stringp = s;
              return (tok);
          }
      } while (sc != 0);
  }
}

int find(const char *str,
         const char *f)
{
  int ret = -1;
  for (int i = 0; i < strlen(str) - strlen(f); i++)
    for (int j = 0; i < strlen(f); j++)
    {
      if (str[i + j] != f[j])
      {
        ret = -1;
        break;
      }
      else
        ret = i;
    }
  return ret;
}

void itoa(int num, char *str, int base)
{
  int i = 0;
  int isNegative = 0;

  // Handle 0 explicitly
  if (num == 0)
  {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  // Handle negative numbers only if base is 10
  if (num < 0 && base == 10)
  {
    isNegative = 1;
    num = -num;
  }

  // Process individual digits
  while (num != 0)
  {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  // Append negative sign for negative numbers
  if (isNegative)
  {
    str[i++] = '-';
  }

  str[i] = '\0'; // Null-terminate the string

  // Reverse the string
  for (int start = 0, end = i - 1; start < end; start++, end--)
  {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
  }

  return;
}

void uitoa(uint32_t num, char *str, int base)
{
  int i = 0;
  int isNegative = 0;

  // Handle 0 explicitly
  if (num == 0)
  {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  // Process individual digits
  while (num != 0)
  {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  // Append negative sign for negative numbers
  if (isNegative)
  {
    str[i++] = '-';
  }

  str[i] = '\0'; // Null-terminate the string

  // Reverse the string
  for (int start = 0, end = i - 1; start < end; start++, end--)
  {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
  }

  return;
}
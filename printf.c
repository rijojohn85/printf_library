/* printf implementation */
#include "printf.h"
#include "x86args.h"
#include <unistd.h>

#define Wait4Char 1 // 00 01
#define Wait4FMT 2  // 00 10

typedef unsigned char State;

/*
 * strlen() - counts chars
 * puts() - puts one string to stdout - for debugging
 * putchar() - prints a single char
 * chardup() -
 */

/*
 * creates a string from a char.
 * eg. chardup('c') ->"c\0"; -> string c with last char \0 to terminate string
 */
char *chardup(const char c) {
  static char buf[2];
  char *p;
  p = buf;
  *p++ = c;
  *p-- = 0;

  return buf;
}

unsigned int r_strlen(const char *str) {
  unsigned int n;
  const char *p;
  /* we start p at str and increment it till we reach a null char and then
   * return n; */
  for (p = str, n = 0; *p; p++, n++)
    ;
  return n;
}

ssize_t r_puts(const char *str) {

  unsigned int n;
  n = r_strlen(str);
  if (n < 1) {
    return -1;
  }
  // fd = 1 is stdout. We are writing str to stdout;
  return write(1, str, n);
}

int r_printf(const char *fmt, ...) {
  unsigned int *p;
  const char *f;
  State s;

  Args(p);
  s = Wait4Char;
  f = fmt;

  do {
    if (s & Wait4Char) {
      switch (*f) {
      case '%':
        s = Wait4FMT;
        break;
      default:
        r_putchar(*f);
      }
    } else if (s & Wait4FMT) {
      switch (*f) {
      case '%':
        r_putchar(*f);
        s = Wait4Char;
        break;
      default:
        s = Wait4Char;
        p++;
        break;
      }
    }
  } while (*(++f));

  return 0;
}

// int main(void) {
//   r_printf("hello world\n");
//   return 0;
// }

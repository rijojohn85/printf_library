/* printf implementation */
#include "printf.h"
#include <stdarg.h>
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

char *r_itoa(int num) {
  /*
   *will hold all digits of int (2147483647) along with sign and \0
   */
  static char str[12];
  // to convert digit to char: int_char[1] -> '1'; int_char[2]='2';
  char *int_char = "0123456789";
  int rem;
  int abs_num = num < 0 ? -num : num;
  // set pointer p to last char of str and set it to \0;
  char *p = &str[11];
  *p-- = '\0';
  do {
    rem = abs_num % 10;
    // we are setting the digit is reverse order in str;
    /*
     * if num = 123;
     * str[11] = '\0'
     * str[10] = '3' <- we start here in the loop
     * str[9] = '2'
     * str[8] = '1';
     */
    *p-- = int_char[rem];
    abs_num /= 10;
  } while (abs_num != 0);
  // add sign if needed.
  if (num < 0) {
    *p-- = '-';
  }
  // need to add one cause p is currently pointing to char before start of num.
  p += 1;
  return p;
}

int r_printf(const char *fmt, ...) {
  const char *f;
  State s;
  va_list argp;
  va_start(argp, fmt);
  s = Wait4Char;
  f = fmt;

  int num;
  do {
    if (s & Wait4Char) {
      switch (*f) {
      // TODO: escape chars
      // case '\%':
      //   r_putchar('%');
      //   break;
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
      case 's':
        // printf("p: %s", (unsigned char *)*p);
        r_puts(va_arg(argp, char *));
        s = Wait4Char;
        break;
      case 'c':
        r_putchar(va_arg(argp, int));
        s = Wait4Char;
        break;
      case 'd':
        // printf("p: %s", (unsigned char *)*p);
        num = va_arg(argp, int);
        r_puts(r_itoa(num));
        s = Wait4Char;
        break;
      default:
        s = Wait4Char;
        break;
      }
    }
  } while (*(++f));
  va_end(argp);
  return 0;
}

// int main(void) {
//   char *p = r_itoa(0);
//
//   r_printf("\%s\n", p);
//   return 0;
// }

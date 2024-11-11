
#include <unistd.h>

/* prints a single char to stdout */
#define r_putchar(c) write(1, chardup(c), 1)
#define atoa(x) #x

/*
 * creates a string from a char.
 * eg. chardup('c') ->"c\0"; -> string c with last char \0 to terminate string
 */
char *chardup(const char c);

/*
 * returns the lenght of the string
 */
unsigned int r_strlen(const char *str);

/*
 * prints a single string to output
 */
ssize_t r_puts(const char *str);

/*printf function - */
int r_printf(const char *fmt, ...);

/* returns a string of the number */
char *r_itoa(int num);

#include "munit/munit.h"
#include "printf.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// int munit_suite_main(const MunitSuite *suite, void *user_data, int argc,
//                      char *const *argv);
MunitResult test_chardup(const MunitParameter params[],
                         void *user_data_or_fixture);
MunitResult test_strlen(const MunitParameter params[],
                        void *user_data_or_fixture);
MunitResult test_puts(const MunitParameter params[],
                      void *user_data_or_fixture);
MunitResult test_puts_0(const MunitParameter params[],
                        void *user_data_or_fixture);
MunitResult test_put_char(const MunitParameter params[],
                          void *user_data_or_fixture);
MunitResult test_printf_without_fmt(const MunitParameter params[],
                                    void *user_data_or_fixture);
MunitResult
test_printf_without_fmt_double_percent(const MunitParameter params[],
                                       void *user_data_or_fixture);
MunitResult test_printf_string(const MunitParameter params[],
                               void *user_data_or_fixture);
MunitResult test_printf_char(const MunitParameter params[],
                             void *user_data_or_fixture);
MunitTest tests[] = {
    {
        "/test_chardup",              // name of test
        test_chardup,                 // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_strlen",               // name of test
        test_strlen,                  // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_puts",                 // name of test
        test_puts,                    // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_puts_0",               // name of test
        test_puts_0,                  // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_putchar",              // name of test
        test_put_char,                // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_printf_without_fmt",   // name of test
        test_printf_without_fmt,      // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_printf_without_fmt_double_percent", // name of test
        test_printf_without_fmt_double_percent,    // test function
        NULL,                                      // setup
        NULL,                                      // teardown
        MUNIT_TEST_OPTION_NONE, NULL,              /* parameters */

    },
    {
        "/test_printf_string",        // name of test
        test_printf_string,           // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {
        "/test_printf_char",          // name of test
        test_printf_char,             // test function
        NULL,                         // setup
        NULL,                         // teardown
        MUNIT_TEST_OPTION_NONE, NULL, /* parameters */

    },
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
};

static const MunitSuite suite = {
    "/printf-tests", /* name of the test suit */
    tests,           NULL, 1, MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, char *argv[]) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
MunitResult test_chardup(const MunitParameter params[],
                         void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  char *str = chardup('r');
  munit_assert_string_equal(str, "r\0");
  return MUNIT_OK;
}
MunitResult test_strlen(const MunitParameter params[],
                        void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  int len = r_strlen("rijojohn\0");
  munit_assert_uint(len, =, 8);
  return MUNIT_OK;
}

char *read_from_file(char *file) {
  FILE *fp = fopen(file, "r");
  size_t offset = 0;  // Index next char goes in the buffer
  size_t bufsize = 4; // initial size of the buffer. preferably multiple of 2
  char *buf;          // the buffer
  int c;              // the character we are going to read.

  buf = malloc(bufsize); // allocate inital buffer

  while (c = fgetc(fp), c != EOF) {
    // check if we're out of room in the buffer account for the extra byte for
    // the NULL terminator
    if (offset == bufsize - 1) { //-1 for the null terminator.
      bufsize *= 2;
      char *new_buf = realloc(buf, bufsize);
      if (new_buf == NULL) {
        free(buf); // bail on error
        perror("Error in realloc\n");
        exit(EXIT_FAILURE);
      }
      buf = new_buf; // successful realloc
    }
    buf[offset++] = c; // add the byte onto the buffer
  }
  // we hit newline or EOF
  //
  // if at EOF and we read no bytes, free the buffer and return NULL to indicate
  // we've hit EOF
  if (c == EOF && offset == 0) {
    free(buf);
    return NULL;
  }

  // shrink to fit
  if (offset < bufsize - 1) {
    char *new_buf = realloc(buf, offset + 1); //+1 for the null terminator
    if (new_buf == NULL) {
      free(buf); // bail on error
      perror("Error in realloc\n");
      exit(EXIT_FAILURE);
    }
    buf = new_buf; // successful realloc
  }
  // Add the nul terminator
  buf[offset] = '\0';
  fclose(fp);
  return buf;
}

MunitResult test_puts(const MunitParameter params[],
                      void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  struct stat st;
  ssize_t bytesWritten = 0;
  // redirect stdout
  freopen("redir.txt", "w", stdout);

  ssize_t res = r_puts("rijojohn\0");

  // assert checking
  stat("redir.txt", &st);
  bytesWritten = st.st_size;
  fclose(stdout);
  char *output = read_from_file("redir.txt");
  munit_assert_string_equal("rijojohn", output);
  munit_assert_long(res, =, bytesWritten);
  return MUNIT_OK;
}

// testing outputing empty string
MunitResult test_puts_0(const MunitParameter params[],
                        void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  ssize_t res = r_puts("");

  // assert checking
  munit_assert_long(res, =, -1);
  return MUNIT_OK;
}
MunitResult test_put_char(const MunitParameter params[],
                          void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  struct stat st;
  ssize_t bytesWritten = 0;
  // redirect stdout
  freopen("redir.txt", "w", stdout);

  ssize_t res = r_putchar('r');

  // assert checking
  stat("redir.txt", &st);
  bytesWritten = st.st_size;
  fclose(stdout);
  char *output = read_from_file("redir.txt");
  munit_assert_string_equal("r", output);
  munit_assert_long(res, =, bytesWritten);
  return MUNIT_OK;
}
MunitResult test_printf_without_fmt(const MunitParameter params[],
                                    void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  struct stat st;
  // redirect stdout
  freopen("redir.txt", "w", stdout);
  int res = r_printf("rijojohn\n");
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);
  char *output = read_from_file("redir.txt");
  munit_assert_string_equal("rijojohn\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);
  return MUNIT_OK;
}

MunitResult
test_printf_without_fmt_double_percent(const MunitParameter params[],
                                       void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  struct stat st;
  // redirect stdout
  freopen("redir.txt", "w", stdout);
  int res = r_printf("rijo %% john\n");
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);
  char *output = read_from_file("redir.txt");
  munit_assert_string_equal("rijo % john\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);
  return MUNIT_OK;
}
MunitResult test_printf_string(const MunitParameter params[],
                               void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  struct stat st;

  /* with single string */

  // redirect stdout
  freopen("redir.txt", "w", stdout);
  int res = r_printf("rijojohn %s\n", "is the best.");
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);
  char *output = read_from_file("redir.txt");
  munit_assert_string_equal("rijojohn is the best.\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);

  /* with multiple string */
  freopen("redir.txt", "w", stdout);
  res = r_printf("%s rijo %s john %s\n", "hello!", "the best", "is the best.");
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);

  output = read_from_file("redir.txt");

  munit_assert_string_equal("hello! rijo the best john is the best.\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);

  /* with string var */
  char *str = "the best";
  freopen("redir.txt", "w", stdout);
  res = r_printf("%s rijo %s john %s\n", "hello!", str, "is the best.");
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);

  output = read_from_file("redir.txt");

  munit_assert_string_equal("hello! rijo the best john is the best.\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);
  return MUNIT_OK;
}
MunitResult test_printf_char(const MunitParameter params[],
                             void *user_data_or_fixture) {
  (void)params;
  (void)user_data_or_fixture;
  struct stat st;

  /* with single string */

  // redirect stdout
  freopen("redir.txt", "w", stdout);
  int res = r_printf("rijojohn %c\n", 'u');
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);
  char *output = read_from_file("redir.txt");
  munit_assert_string_equal("rijojohn u\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);

  /* with multiple string */
  freopen("redir.txt", "w", stdout);
  res = r_printf("%c rijo %c john %c\n", 'a', 'b', 'c');
  // assert checking
  stat("redir.txt", &st);
  fclose(stdout);

  output = read_from_file("redir.txt");

  munit_assert_string_equal("a rijo b john c\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);
  /* with var */
  char c = 'a';
  freopen("redir.txt", "w", stdout);
  // assert checking
  res = r_printf("%c rijo %c john %c\n", c, c, c);
  stat("redir.txt", &st);
  fclose(stdout);

  output = read_from_file("redir.txt");

  munit_assert_string_equal("a rijo a john a\n", output);
  // munit_assert_int(res, =, 0);
  assert(res == 0);
  return MUNIT_OK;
}

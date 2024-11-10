/*
 *
 * x86args.h - Variable-len arguments marco for IA32
 *
 * <rijojohn85@gmail.com>
 * 2024
 *
 */

#ifndef x86ARGS
#define x86ARGS

/*
 * Args :: Int* -> Int*
 */

#define Args(arg)                                                              \
  unsigned int _x;                                                             \
  __asm("mov %%ebp,%0" : "=r"(_x));                                            \
  _x += 8;                                                                     \
  arg = (unsigned int *)_x
#endif

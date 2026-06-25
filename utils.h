



#define TRUE  (1)
#define FALSE (0)
#ifndef THREADED_CUDA
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
extern int debug;
extern void exit_error(char *format,...);
extern void *myalloc(char *str,size_t size);
extern void debug_printf(char *format,...);

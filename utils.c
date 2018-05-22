/*
This file is part of fundamental a brute force searcher
for relationships between constants & formulae for sequences.
Copyright (C) 2004  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

It is licensed under GPL v2.1.
*/
#include <stdarg.h>
#include "utils.h"

int debug=FALSE;
void exit_error(char *format,...)
{
   va_list ap;

   va_start(ap,format);
   vfprintf(stderr,format,ap);
   va_end(ap);
   exit(-1);
}


void *myalloc(char *str,size_t size)
{
   void *retval=(void *)malloc(size);
   if(!retval)
      exit_error("ran out of memory allocating %s\n",str);
   return(retval);
}




void debug_printf(char *format,...)
{
   va_list ap;

   if(debug)
   {
      va_start(ap,format);
      vfprintf(stdout,format,ap);
      va_end(ap);
   }
}

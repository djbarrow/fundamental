/*
This file is part of fundamental a brute force searcher
for relationships between constants & formulae for sequences.
Copyright (C) 2004  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

It is licensed under GPL v2.1.
*/
#include "fundamental.h"
#include <math.h>
#ifdef SEQUENCE_HUNTER
#if 0
int sequence_func(number_t *retnum,dimension_t *array_indices)
{

   dimension_t curr_idx;
   dimension_t *temp_indices=alloca(sizeof(dimension_t)*NUM_SEQUENCE_DIMENSIONS);
   if(array_indices[0]==0||array_indices[1]<2)
      *retnum=3;
   else
   {
      temp_indices[0]=array_indices[0]-1;
      temp_indices[1]=array_indices[1]-2;
      *retnum=*(get_array_member(temp_indices))+3;
   }
   return 0;
}
#endif

#if 0
int sequence_func(number_t *retnum,dimension_t *array_indices)
{
   int numprimes=0;
   int idx1,idx2;

   for(idx1=1;;idx1++)
   {
      for(idx2=2;idx2<idx1;idx2++)
      {
	 if((idx1%idx2)==0)
	    goto Skip;
      }
      if(numprimes==array_indices[0])
      {
	 *retnum=idx1;
	 break;
      }
      numprimes++;
     Skip:
   }
   return 0;
}
#endif

#if 0
int sequence_func(number_t *retnum,dimension_t *array_indices)
{
#if 0
   int idx2;

   for(idx2=2;idx2<array_indices[0];idx2++)
   {
      if((array_indices[0]%idx2)==0)
      {
	 *retnum=0;
	 return 0;
      }
   }
   *retnum=1;
   return 0;
#endif
   int i;
   long long fact=1;
   for(i=1;i<=array_indices[0];i++)
      fact=fact*(i+1);
   *retnum=fact;
   return 0;
}
#endif
#if 0
double f_of_x(double x)
{
  return sin(x*x);
}
#define widthinc (1.0/1024.0)
#define PI 3.14159265
#define NUM_RETVALS 10
int init=FALSE;
double retvals[NUM_RETVALS+1]={0.0,};
void fundfill()
{
  int i;
  double j,y0,y1,intadd;
  double next_crossing=1,x0;

  for(i=0;i<NUM_RETVALS;i++)
    {
      for(j=0;j<1;j+=widthinc)
	{
	  y0=f_of_x(i+j);
	  y1=f_of_x(i+j+widthinc);
	  if(y0>=0)
	    {
	      if(y1>=0)
		{
		  intadd=((y0+y1)/2)*widthinc;
		}
	      else
		{
		  x0=sqrt(next_crossing++*PI);
		  intadd=(y0*(x0-(i+j))/2)-(y1*((i+j+widthinc)-x0)/2);
		}
	    }
	  else
	    {
	      if(y1<0)
		{
		  intadd=((y0+y1)/2)*widthinc;
		}
	      else
		{
		  x0=sqrt(next_crossing++*PI);
		  intadd=(y1*((i+j+widthinc)-x0)/2)-(y0*(x0-(i+j))/2);
		}
	    }
	  retvals[i+1]+=intadd;
	}
      if(i>=1)
	retvals[i+1]+=retvals[i];
    }
  init=TRUE;
}
#endif
#if 0
int sequence_func(number_t *retnum,dimension_t *array_indices)
{
#if 0
  *retnum=/*sin(array_indices[0]*array_indices[0])+*/(2*cos(array_indices[0]));
  return 0;
#endif
  if(!init)
    {
      fundfill();
    }
  if(array_indices[0]<=NUM_RETVALS)
    {
      *retnum=retvals[array_indices[0]];
      return 0;
    }
  return 1;
}
#endif
#if 1
int sequence_func(number_t *retnum,dimension_t *array_indices)
{
   *retnum=array_indices[0]+array_indices[1];
   return 0;
}
#endif
#if 0
int sequence_func(result_t *retnum,dimension_t *array_indices)
{
   *retnum=array_indices[0]+array_indices[1];
   return 0;
}
#endif
#if 0
int sequence_func(result_t *retnums,dimension_t *array_indices)
{
#ifndef NUM_ANSWERS
   *retnums=alloc_result(2);
   (*retnums)->answer[0]=array_indices[0]*2;
   (*retnums)->answer[1]=array_indices[0]*3;
#else
   retnums->answer[0]=array_indices[0]*2;
   retnums->answer[1]=array_indices[0]*3;
#endif

   return 0;
}
#endif
#if 0
int sequence_func(result_t *retnum,dimension_t *array_indices)
{
   *retnum=sin(array_indices[0]);
   return 0;
}
#endif
#if 0
int sequence_func(result_t *retnum,dimension_t *array_indices)
{
   *retnum=exp(array_indices[0]);
   return 0;
}
#endif
#if 0
int sequence_func(result_t *retnum,dimension_t *array_indices)
{
   *retnum=array_indices[0]*3+2;
   return 0;
}
#endif
#endif /* SEQUENCE_HUNTER */

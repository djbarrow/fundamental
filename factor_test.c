#include "fundamental.h"
#define MAX_N_FACTORS 128
typedef unsigned long long uintmax_t;

static int factor (uintmax_t n0, int max_n_factors, uintmax_t *factors)
{
  register uintmax_t n = n0, d, q;
  int n_factors = 0;

  if (n < 1)
    return n_factors;

  while (n % 2 == 0)
    {
      assert (n_factors < max_n_factors);
      factors[n_factors++] = 2;
      n /= 2;
    }

  /* The exit condition in the following loop is correct because
     any time it is tested one of these 3 conditions holds:
      (1) d divides n
      (2) n is prime
      (3) n is composite but has no factors less than d.
     If (1) or (2) obviously the right thing happens.
     If (3), then since n is composite it is >= d^2. */

  d = 3;
  do
    {
      q = n / d;
      while (n == q * d)
	{
	  assert (n_factors < max_n_factors);
	  factors[n_factors++] = d;
	  n = q;
	  q = n / d;
	}
      d += 2;
    }
  while (d <= q);

  if (n != 1 || n0 == 1)
    {
      assert (n_factors < max_n_factors);
      factors[n_factors++] = n;
    }

  return n_factors;
}

#if 1
#if NUM_SEQUENCE_DIMENSIONS!=1 || !defined(MULTIPLE_RESULTS)
#error "Bad factorisation configuration"
#endif
int sequence_func(result_t *retnums,dimension_t *array_indices)
{
   uintmax_t factors[MAX_N_FACTORS];
   int i,n_factors;
   n_factors = factor ((uintmax_t)array_indices[0], MAX_N_FACTORS, factors);
#ifdef SPARSE_ARRAY_INDICES
#if NUM_ANSWERS!=2
#error "NUM_ANSWERS defined wrong"
#endif
   if(n_factors==2)
      *retnums=alloc_result();
   else
   {
      *retnums=NULL;
      return 0;
   }
#else
   *retnums=alloc_result(n_factors);
#endif
   for(i=0;i<n_factors;i++)
      (*retnums)->answer[i]=(number_t)factors[i];
   return 0;
}
#endif

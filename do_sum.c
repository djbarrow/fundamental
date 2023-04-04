/*
This file is part of fundamental a brute force searcher
for relationships between constants & formulae for sequences.
It is a source file for the reverse polish notation RPN calculation 
code in the project.
Copyright (C) 2018  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

It licensed is under GPL v2.1
*/

#include "fundamental.h"
#include "utils.h"
#include "do_sum.h"
#ifdef HAVE_GCD_OP
uint_t euclid_calc_gcd(uint_t x,uint_t y)
{
   uint_t temp;

   for(;;)
   {
      if(x==y)
	 return x;
      if(x<y)
      {
	temp=((uint_t)y%(uint_t)x);
	 if(temp==0)
	    return(x);
	 else
	    y=temp;
      }
      else
      {
	temp=((uint_t)x%(uint_t)y);
	 if(temp==0)
	    return(y);
	 else
	    x=temp;
      }
   }
}
#endif

#ifdef HAVE_FACTORIAL_OP
number_t *factorials=NULL;
int max_factorial;
void init_factorials()
{
   int n,pass;
   number_t factorial;

   for(pass=1;pass<=2;pass++)
   {
      uint_t factorial=1,oldfactorial=1;
      for(n=1;pass==1||n<=max_factorial;n++)
      {
	oldfactorial=factorial;
	factorial=factorial*n;
	 if(pass==2)
	    factorials[n]=factorial;
	 else if((factorial/n)!=oldfactorial)
	    break;
      }
      if(pass==1)
      {
	 max_factorial=n-1;
	 factorials=myalloc("factorials",sizeof(uint_t)*max_factorial);
	 factorials[0]=1;
      }
   }
}
#endif

#ifdef HAVE_BINARY_OPERATORS
/* If this is modified fix simplify_binary_sum too */
int do_binary_sum(number_t *result_ptr,number_t *operand,stack_entry *curr)
{
   operation op=curr->val;
   switch(op)
   {
#ifdef HAVE_ADDITION_OP
      case addition_op:
	 *result_ptr=operand[0]+operand[1];
	 break;
#endif
#ifdef HAVE_SUBTRACTION_OP
      case subtraction_op:
	 *result_ptr=operand[0]-operand[1];
	 break;
#endif
#ifdef HAVE_MULTIPLY_OP
      case multiply_op:
	 *result_ptr=SIGN_EXTEND(operand[0])*SIGN_EXTEND(operand[1]);
	 break;
#endif
#ifdef HAVE_DIVIDE_OP
      case divide_op:
	 if(operand[1]==0)
	    abort_sum(-1,"division by zero aborting sum "
		      NUMBER_FORMAT"/"NUMBER_FORMAT,operand[0],operand[1]);
	 *result_ptr=SIGN_EXTEND(operand[0])/SIGN_EXTEND(operand[1]);
	 break;
#endif
#ifdef HAVE_POWER_OP
      case power_op:  
#ifdef NUM_INTEGER_BITS
#ifndef SIGNED_OPERATION
	 if(IS_NEGATIVE(operand[1]))
	    abort_sum(-1,"int power less than 0 aborting sum pow("
		      NUMBER_FORMAT","NUMBER_FORMAT")",operand[0],operand[1]);
#endif
	 if(operand[0]==0)
	 {
	    *result_ptr=0;
	    break;
	 }
	 *result_ptr=1;
	 if(operand[0]==1)
	    break;
	 if(operand[0]==-1)
	 {
	    if(operand[1]&1)
	       *result_ptr=-1;
	    break;
	 }
	 {
	    number_t idx,oldresult=1;
	    for(idx=1;idx<=operand[1];idx++)
	    {
	       *result_ptr*=operand[0];
	       if((*result_ptr/operand[0])!=oldresult)
		  abort_sum(-1,"overflow aborting sum pow("
			    NUMBER_FORMAT","NUMBER_FORMAT")",operand[0],operand[1]); 
	       oldresult=*result_ptr;
	    }
	 }

#else
	 *result_ptr=pow(operand[0],operand[1]);
#endif
	 break;
#endif
#ifdef HAVE_MODULO_OP
      case modulo_op:
	 if(operand[1]==0)
	    abort_sum(-1,"modulo div by zero aborting sum "NUMBER_FORMAT
		      "%%"NUMBER_FORMAT,operand[0],operand[1]);
#ifdef NUM_INTEGER_BITS
	 *result_ptr=operand[0]%operand[1];
#else
	 {
	    number_t n=operand[0]/operand[1];
	    int_t    n1;
	    if(number_to_int_t(&n,&n1))
	       abort_sum(-1,"number out of range\n");

	    *result_ptr=n-n1;
	 }
#endif
	 break;
#endif
#ifdef HAVE_AND_OP
      case and_op:
	*result_ptr=(uint_t)operand[0]&(uint_t)operand[1];
	 break;
#endif
#ifdef HAVE_OR_OP
      case or_op:
	*result_ptr=(uint_t)operand[0]|(uint_t)operand[1];
	 break;
#endif
#ifdef HAVE_XOR_OP
      case xor_op:
	*result_ptr=(uint_t)operand[0]^(uint_t)operand[1];
	 break;
#endif
#ifdef HAVE_LOG_SHIFT_OP
      case log_shift_op:
#ifdef SIGNED_OPERATION
	 if(IS_NEGATIVE(operand[1]))
	    *result_ptr=((uint_t)operand[0])>>(MAKE_POSITIVE(operand[1]));
	 else
#endif
	    *result_ptr=((uint_t)operand[0])<<operand[1];
	 break;
#endif
#ifdef HAVE_LOG_RSHIFT_OP
      case log_rshift_op:
	*result_ptr=(uint_t)operand[0]>>(uint_t)operand[1];
	 break;
#endif
#ifdef HAVE_LOG_LSHIFT_OP
      case log_lshift_op:
	*result_ptr=(uint_t)operand[0]<<(uint_t)operand[1];
	 break;
#endif
#ifdef HAVE_ARITH_SHIFT
      case arith_shift:
	 if(IS_NEGATIVE(operand[1]))
	   *result_ptr=SIGN_EXTEND((uint_t)operand[0])>>(MAKE_POSITIVE((uint_t)operand[1]));
	 else
	   *result_ptr=(uint_t)operand[0]<<(uint_t)operand[1];
	 break;
#endif
#ifdef HAVE_ROTATE_OP
      case rotate:
      {
	 uint_t rotval;

#ifdef SIGNED_OPERATION
	 if(IS_NEGATIVE(operand[1]))
	 {
	   rotval=(MAKE_POSITIVE((uint_t)operand[1]))&(NUM_INTEGER_BITS-1);
	    *result_ptr=((((uint__t)operand[0])>>rotval)|
		(((uint_t)operand[0])<<(NUM_INTEGER_BITS-rotval)));
	 }
	 else
#endif
	 {
	    rotval=operand[1]&(NUM_INTEGER_BITS-1);
	    *result_ptr=((((uint_t)operand[0])<<rotval)|
	       (((uint_t)operand[0])>>(NUM_INTEGER_BITS-rotval)));
	 }
      }
      break;
#endif
#ifdef HAVE_LROTATE_OP
      case lrotate_op:
      {
	  number_t rotval;

	  rotval=operand[1]&(NUM_INTEGER_BITS-1);
	  *result_ptr=((((uint_t)operand[0])<<rotval)|
		       (((uint_t)operand[0])>>(NUM_INTEGER_BITS-rotval)));
      }
      break;
#endif
#ifdef HAVE_RROTATE_OP
      case rrotate_op:
      {
	 number_t rotval;

	 rotval=operand[1]&(NUM_INTEGER_BITS-1);
	 *result_ptr=((((uint_t)operand[0])>>rotval)|
		      (((uint_t)operand[0])<<(NUM_INTEGER_BITS-rotval)));
      }
      break;
#endif
#ifdef  HAVE_MASK_OP
      case mask_op:
      {
	 number_t mask[2],shiftval[2];

	 shiftval[0]=((operand[0])&NUM_INTEGER_BITS-1);
	 shiftval[1]=((operand[1])&NUM_INTEGER_BITS-1);
	 mask[0]=~(((uint_t)-1)<<shiftval[0]);
	 mask[1]=~(((uint_t)-1)<<shiftval[1]);
	 if(shiftval[0]<shiftval[1])
	    *result_ptr=mask[0]&mask[1];
	 else
	    *result_ptr=(~mask[0])|mask[1];
      }
      break;
#endif
#ifdef HAVE_GETBIT_OP
      case getbit_op:
	 *result_ptr=(operand[0]>>(operand[1]&(NUM_INTEGER_BITS-1)))&1;
      break;
#endif
#ifdef HAVE_LN_OP
      case ln_op:
	 /* log(operand[1]) to base operand[0] = log(operand[1])/ log operand[0] */
	 if(operand[0]<0||operand[1]<0||operand[0]==1)
	    abort_sum(-1,"can't do log("NUMBER_FORMAT")/log("NUMBER_FORMAT")",operand[1],operand[0]);
	 *result_ptr=log(operand[1])/log(operand[0]);
	 break;
#endif
#ifdef  HAVE_GCD_OP
      case gcd_op:
	if(SIGN_EXTEND((uint_t)operand[0])<=0||SIGN_EXTEND((uint_t)operand[1])<=0)
	    abort_sum(-1,"can't do gcd("NUMBER_FORMAT","NUMBER_FORMAT")",operand[0],operand[1]);
	 *result_ptr=euclid_calc_gcd(operand[0],operand[1]);
	 break;
#endif
#ifdef HAVE_EQ_COND  
      case eq_cond:
	 *result_ptr=(operand[0]==operand[1] ? 1:0);
	 break;
#endif
#ifdef HAVE_GT_COND
      case gt_cond:
	 *result_ptr=(SIGN_EXTEND(operand[0])>SIGN_EXTEND(operand[1]) ? 1:0);
	 break;    
#endif
#ifdef HAVE_GE_COND  
      case ge_cond:
	 *result_ptr=(SIGN_EXTEND(operand[0])>=SIGN_EXTEND(operand[1]) ? 1:0);
	 break;
#endif
#ifdef HAVE_LT_COND
      case lt_cond:
	 *result_ptr=(SIGN_EXTEND(operand[0])<SIGN_EXTEND(operand[1]) ? 1:0);
	 break;
#endif
#ifdef HAVE_LE_COND
      case le_cond:
	 *result_ptr=(SIGN_EXTEND(operand[0])<=SIGN_EXTEND(operand[1]) ? 1:0);
	 break;
#endif
#ifdef HAVE_MIN_OP
      case min_op:
	 *result_ptr=(SIGN_EXTEND(operand[0])<=SIGN_EXTEND(operand[1]) ? operand[0]:operand[1]);
	 break;
#endif
#ifdef HAVE_MAX_OP
      case max_op:
	 *result_ptr=(SIGN_EXTEND(operand[0])>SIGN_EXTEND(operand[1]) ? operand[0]:operand[1]);
	 break;
#endif
      default:
	 return -2;
   }
   return 0;
} 
#endif

/* if this is modified fix simplify_sum too */
int do_sum(number_t **result_stack_head_ptrptr,stack_entry *curr)
{
   number_t *operand,*result_ptr;
   operation op=curr->val; 
   depth_t depth_change=1-op_depth[curr->val];
   int retval;
   *result_stack_head_ptrptr+=depth_change;
   operand=result_ptr=(*result_stack_head_ptrptr)-1;
  
   switch(op)
   {
#ifdef HAVE_FACTORIAL_OP
      case factorial_op:
	 if(SIGN_EXTEND(operand[0])<0||SIGN_EXTEND(operand[0])>max_factorial)
	    abort_sum(-1,"factorial out of bounds "
		      NUMBER_FORMAT,operand[0]);
	 *result_ptr=
	    factorials[
#ifndef NUM_INTEGER_BITS
	       (int_t)
#endif
	       operand[0]];
	 break;
#endif

#ifdef HAVE_ABS_OP
      case abs_op:
	 if(IS_NEGATIVE(operand[0]))
	    *result_ptr=MAKE_POSITIVE(operand[0]);
	 break;
#endif
#ifdef HAVE_FLOOR_OP
      case floor_op:
	 *result_ptr=floor(operand[0]);
	 break;
#endif
#ifdef HAVE_CEIL_OP
      case ceil_op:
	 *result_ptr=ceil(operand[0]);
	 break;
#endif
#ifdef HAVE_RINT_OP
      case rint_op:
	 *result_ptr=rint(operand[0]);
	 break;
#endif
#ifdef  HAVE_SIN_OP
      case sin_op:
	 *result_ptr=sin(operand[0]);
	 break;
#endif
#ifdef HAVE_COS_OP
      case cos_op:
	 *result_ptr=cos(operand[0]);
	 break;
#endif
#ifdef HAVE_TAN_OP
      case tan_op:
	 *result_ptr=tan(operand[0]);
	 break;
#endif
#ifdef HAVE_ONES_COMPLIMENT_OP
      case ones_compliment_op:
	 *result_ptr=~operand[0];
	 break;

#endif
#ifdef HAVE_NOT_OP
      case not_op:
	 *result_ptr=!operand[0];
	 break;
#endif
      default:
#ifdef HAVE_BINARY_OPERATORS
	 if((retval=do_binary_sum(result_ptr,operand,curr))==-2)
#endif
	    exit_error("bad operation %d",op);
#ifdef HAVE_BINARY_OPERATORS
	 else
	    return retval;
#endif
   }
   return(0);
}

/*
This file is part of fundamental a brute force searcher
for relationships between constants & formulae for sequences.
Copyright (C) 2004  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

It is under MYPL which is entirely compatible with GPL.
Pay me what it is worth.
*/
#include "fundamental.h"
#include <stdio.h>
#include "utils.h"
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#ifdef HAVE_PROGRESS
#include <signal.h>
#include <sys/time.h>
struct timeval *starttime;
#endif


#ifdef SEQUENCE_HUNTER
dimension_t
#ifdef HAVE_FUNCTIONS 
min_seed=0,max_seed=0,
#endif
*sequence_dimension,sequence_array_size,*sequence_dimension_multiplier;
dimension_t *array_indices,*temp_dimensions;
result_t *sequence_array;
#ifndef NUM_SEQUENCE_DIMENSIONS
dimension_t NUM_SEQUENCE_DIMENSIONS=1;
#endif
#endif
#if defined(MULTIPLE_RESULTS)
dimension_t *retvals;
#ifndef NUM_ANSWERS
dimension_t MAX_NUM_RESULTS;
#endif
#endif



#ifdef HAVE_CONSTANTS_FILE
fundamental_constant *head=NULL,**fundamental_list=NULL;
int num_constants;
#endif

int hi_int,total,max_stack_depth,curr_depth1;
#if !defined(NUM_INTEGER_BITS) && !defined(ERROR_OP)
number_t error_tolerance=0.0001;
#endif

sum_t *sum;


int num_answers=0,max_num_answers=-1;



#ifdef SEED
#define MIN_SEED (SEED)
#else
#define MIN_SEED (0)
#endif


depth_t op_depth[num_operators]=
{
#ifdef HAVE_UNARY_OPERATORS
   [first_unary_op ... last_unary_op]=1,
#endif
#ifdef HAVE_BINARY_OPERATORS
   [first_binary_op ... last_binary_op]=2,
#endif
};

operation first_operator[max_operator_depth]=
{
#ifdef HAVE_UNARY_OPERATORS
   first_unary_op,
#else
   0,
#endif
#ifdef HAVE_BINARY_OPERATORS
   first_binary_op
#endif
};





#ifdef HAVE_ERROR_MEASUREMENTS
error_t error_val[NUM_ERROR_MEASUREMENTS];
/* max_error_list_size must be >=2 */
int    max_error_list_size=100;
int    num_error_list_elements[NUM_ERROR_MEASUREMENTS];         
struct list_head error_list[NUM_ERROR_MEASUREMENTS];
int    refinement_depth=8;
int    next_refinement_depth;
int    skip_depth=0;
#define SKIP_DEPTH skip_depth
#define MAX_STACK_DEPTH next_refinement_depth

int error_sums_good()
{
   int idx;

   for(idx=0;idx<NUM_ERROR_MEASUREMENTS;idx++)
      if((num_error_list_elements[idx]<max_error_list_size||error_val[idx]<((error_list_element *)error_list[idx].prev)->error_val))
	 return 1;
}

int add_sum_to_list(int idx)
{
   error_list_element *element;
   struct list_head *curr_error_list=&error_list[idx];
   error_t curr_error_val=error_val[idx];
   error_list_element *new_entry=NULL;

   if(list_empty(curr_error_list))
      goto add_entry;
   else
   {
      if(num_error_list_elements[idx]>=max_error_list_size&&curr_error_val>((error_list_element *)curr_error_list->prev)->error_val)
	 return 1;
      list_for_each(((struct list_head *)element),curr_error_list)
      {
	 if(element->error_val>curr_error_val||((struct list_head *)element)->next==curr_error_list)
	 {
	    if(element->error_val<=curr_error_val)
	    {
	       /* Special case for adding an element immeadiately after
                 the first & only element in the list. */
	       element=(error_list_element *)curr_error_list;
	    }
	    if(num_error_list_elements[idx]>=max_error_list_size)
	    {
	       new_entry=(error_list_element *)curr_error_list->prev;	    
	       list_del((struct list_head *)new_entry);
	       if(new_entry==element)
		  element=(error_list_element *)curr_error_list;
	    }
	    goto add_entry;
	 }
      }
   }
  add_entry:;   
   if(!new_entry)
   {
      new_entry=(error_list_element *)myalloc("new_entry",offsetof(error_list_element,sum.stack[max_stack_depth]));
      num_error_list_elements[idx]++; 
   }
   new_entry->error_val=curr_error_val;
   new_entry->processed=FALSE;
   memcpy(&new_entry->sum,sum,offsetof(sum_t,stack[sum->stack_depth]));
   if(list_empty(curr_error_list))
      list_add((struct list_head *)new_entry,curr_error_list);
   else
      __list_add((struct list_head *)new_entry,
		 element->list.prev,
		 (struct list_head *)element);
   return 0;
}
#else
#define MAX_STACK_DEPTH max_stack_depth
#define SKIP_DEPTH    0
#endif




#ifdef HAVE_GCD_OP
number_t euclid_calc_gcd(number_t x,number_t y)
{
   number_t temp;

   for(;;)
   {
      if(x==y)
	 return x;
      if(x<y)
      {
	 temp=(y%x);
	 if(temp==0)
	    return(x);
	 else
	    y=temp;
      }
      else
      {
	 temp=(x%y);
	 if(temp==0)
	    return(y);
	 else
	    x=temp;
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
		      "%"NUMBER_FORMAT,operand[0],operand[1]);
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
	 *result_ptr=operand[0]&operand[1];
	 break;
#endif
#ifdef HAVE_OR_OP
      case or_op:
	 *result_ptr=operand[0]|operand[1];
	 break;
#endif
#ifdef HAVE_XOR_OP
      case xor_op:
	 *result_ptr=operand[0]^operand[1];
	 break;
#endif
#ifdef HAVE_LOG_SHIFT_OP
      case log_shift_op:
#ifdef SIGNED_OPERATION
	 if(IS_NEGATIVE(operand[1]))
	    *result_ptr=((unsigned_number_t)operand[0])>>(MAKE_POSITIVE(operand[1]));
	 else
#endif
	    *result_ptr=((unsigned_number_t)operand[0])<<operand[1];
	 break;
#endif
#ifdef HAVE_LOG_RSHIFT_OP
      case log_rshift_op:
	 *result_ptr=operand[0]>>operand[1];
	 break;
#endif
#ifdef HAVE_LOG_LSHIFT_OP
      case log_lshift_op:
	 *result_ptr=operand[0]<<operand[1];
	 break;
#endif
#ifdef HAVE_ARITH_SHIFT
      case arith_shift:
	 if(IS_NEGATIVE(operand[1]))
	    *result_ptr=SIGN_EXTEND(operand[0])>>(MAKE_POSITIVE(operand[1]));
	 else
	    *result_ptr=operand[0]<<operand[1];
	 break;
#endif
#ifdef HAVE_ROTATE_OP
      case rotate:
      {
	 number_t rotval;

#ifdef SIGNED_OPERATION
	 if(IS_NEGATIVE(operand[1]))
	 {
	    rotval=(MAKE_POSITIVE(operand[1]))&(NUM_INTEGER_BITS-1);
	    *result_ptr=((((unsigned_number_t)operand[0])>>rotval)|
		(((unsigned_number_t)operand[0])<<(NUM_INTEGER_BITS-rotval)));
	 }
	 else
#endif
	 {
	    rotval=operand[1]&(NUM_INTEGER_BITS-1);
	    *result_ptr=((((unsigned_number_t)operand[0])<<rotval)|
	       (((unsigned_number_t)operand[0])>>(NUM_INTEGER_BITS-rotval)));
	 }
      }
      break;
#endif
#ifdef HAVE_LROTATE_OP
      case lrotate_op:
      {
	  number_t rotval;

	  rotval=operand[1]&(NUM_INTEGER_BITS-1);
	  *result_ptr=((((unsigned_number_t)operand[0])<<rotval)|
		       (((unsigned_number_t)operand[0])>>(NUM_INTEGER_BITS-rotval)));
      }
      break;
#endif
#ifdef HAVE_RROTATE_OP
      case rrotate_op:
      {
	 number_t rotval;

	 rotval=operand[1]&(NUM_INTEGER_BITS-1);
	 *result_ptr=((((unsigned_number_t)operand[0])>>rotval)|
		      (((unsigned_number_t)operand[0])<<(NUM_INTEGER_BITS-rotval)));
      }
      break;
#endif
#ifdef  HAVE_MASK_OP
      case mask_op:
      {
	 number_t mask[2],shiftval[2];

	 shiftval[0]=((operand[0])&NUM_INTEGER_BITS-1);
	 shiftval[1]=((operand[1])&NUM_INTEGER_BITS-1);
	 mask[0]=~(((unsigned_number_t)-1)<<shiftval[0]);
	 mask[1]=~(((unsigned_number_t)-1)<<shiftval[1]);
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
	 if(SIGN_EXTEND(operand[0])<=0||SIGN_EXTEND(operand[1])<=0)
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


number_t *result_stack;

#if MAX_NUM_LOOPVARS
int curr_num_loopvars1;
loopvar_t loopvar[MAX_NUM_LOOPVARS];
stack_tag last_number;



#define RESULT_STACK_START (1)

void inc_loopfunc(loopvar_t *loopvar_ptr)
{
   (*loopvar_ptr)++;
}

void dec_loopfunc(loopvar_t *loopvar_ptr)
{
   (*loopvar_ptr)--;
}


void lshift_loopfunc(loopvar_t *loopvar_ptr)
{
   (*loopvar_ptr)<<=1;
}

void rshift_loopfunc(loopvar_t *loopvar_ptr)
{
   (*loopvar_ptr)>>=1;
}




looptype_t looptypes[NUM_LOOPTYPES+1]=
{
#ifdef HAVE_INC_LOOPFUNC
   {0,MAX_LOOPVAL,inc_loopfunc,"++"},
#endif
#ifdef HAVE_DEC_LOOPFUNC
   {MAX_LOOPVAL,0,dec_loopfunc,"--"},
#endif
#ifdef HAVE_LSHIFT_NBITS_LOOPFUNC
   {1,(1<<(MAX_LOOPVAL_NUM_INTEGER_BITS-1)),lshift_loopfunc,"=<<1"},
#endif
#ifdef HAVE_RSHIFT_NBITS_LOOPFUNC
   {(1<<(MAX_LOOPVAL_NUM_INTEGER_BITS-1)),1,rshift_loopfunc,"=>>1"},
#endif
#ifdef HAVE_INC_PWR_NBITS_LOOPFUNC
   {0,(1ULL<<MAX_LOOPVAL_NUM_INTEGER_BITS)-1,inc_loopfunc,"++"},
#endif
#ifdef HAVE_DEC_PWR_NBITS_LOOPFUNC
   {(1ULL<<MAX_LOOPVAL_NUM_INTEGER_BITS)-1,0,dec_loopfunc,"--"},
#endif
#if ZERO_MAX_LOOPVAL
   /* The zero causes a lot of division by zeros & sums aborted */
   {0,ZERO_MAX_LOOPVAL,inc_loopfunc,"++"},
#endif
#if ONE_MAX_LOOPVAL
   {1,ONE_MAX_LOOPVAL,inc_loopfunc,"++"},
#endif
   {0,-1,NULL,""}
 };

void init_looptype_indices()
{
   memset(sum->looptype_idx,0,sizeof(*sum->looptype_idx)*sum->num_loopvars);
}

int increment_looptype_indices()
{
   int idx;
   int *curr_looptype_idx=&sum->looptype_idx[0],*prev_looptype_idx=NULL;

   for(idx=0;idx<sum->num_loopvars;idx++)
   {
      if(*curr_looptype_idx<(NUM_LOOPTYPES-1))
      {
	 (*curr_looptype_idx)++;
	 break;
      }
      else
      {
	 /* eliminate duplicate looptypes */
	 *curr_looptype_idx=(prev_looptype_idx ? *prev_looptype_idx:0 );
      }
      prev_looptype_idx=curr_looptype_idx++;
   }
   return (idx>=sum->num_loopvars);
}

void init_loopvars()
{
   int idx;

   for(idx=0;idx<sum->num_loopvars;idx++)
      loopvar[idx]=looptypes[sum->looptype_idx[idx]].initial_val;
} 


int increment_loopvars()
{
   int idx;
   loopvar_t *curr_loopvar;
   looptype_t *curr_looptype;
   int retval;

   for(idx=0;idx<sum->num_loopvars;idx++)
   {
      curr_loopvar=&loopvar[idx];
      curr_looptype=&looptypes[sum->looptype_idx[idx]];
      if(*curr_loopvar==curr_looptype->final_val)
	 *curr_loopvar=curr_looptype->initial_val;
      else
      {
	 if((retval=do_binary_sum(&result_stack[0],&result_stack[0],&sum->loopvar_stack[idx])))
	    retval=-1;
	 curr_looptype->loopfunc(curr_loopvar);
	 break;
      }
   }
   return((idx>=sum->num_loopvars) ? 1:0);
}

void init_loopvar_operators()
{
   stack_entry *curr;
   int idx;

   for(idx=0;idx<sum->num_loopvars;idx++)
   {
      curr=&sum->loopvar_stack[idx];
#ifdef SIGNED_OPERATION
      curr->minus=FALSE;
#endif
      curr->tag=arithmetic_operation_tag;
      curr->val=first_binary_op;
   }
}

int increment_loopvar_operators()
{
   int idx;
   stack_entry *curr;

   for(idx=0;idx<sum->num_loopvars;idx++)
   {
      curr=&sum->loopvar_stack[idx];
      curr->val++;
      if(curr->val>last_binary_op)
	 curr->val=first_binary_op;
      else
	 break;
   }
   return(idx>=curr_num_loopvars1);
}

int check_all_loopvars_used()
{
   int idx;
   stack_entry *curr;
   int loopvar_used_cnt[MAX_NUM_LOOPVARS];

   memset(loopvar_used_cnt,0,sizeof(loopvar_used_cnt[0])*sum->num_loopvars);
   for(idx=0;idx<sum->stack_depth;idx++)
   {
      curr=&sum->stack[idx];
      if(curr->tag==loopvar_tag)
	 loopvar_used_cnt[curr->val]++;
   }
   for(idx=0;idx<sum->num_loopvars;idx++)
   {
      if(loopvar_used_cnt[idx]==0)
	 return 0;
   }
   return 1;
}


#ifdef HAVE_RESULT_LOOPVAR
void init_result_loopvar()
{
   sum->result_loopvar=0;
} 

int increment_result_loopvar()
{
   int retval=0;
#ifdef BOOLEAN_RESULT_LOOPVAR
   if(sum->result_loopvar==(number_t)(-1LL
#ifdef RESULT_MASK
			&RESULT_MASK
#endif
	 ))
   {
      sum->result_loopvar=0;
      retval=1;
   }
   else
      sum->result_loopvar=-1LL;
#else
   if(sum->result_loopvar!=0)
      sum->result_loopvar=-sum->result_loopvar;
   if(sum->result_loopvar>=0)
   {
      
      sum->result_loopvar++;
      if(sum->result_loopvar>hi_int)
      {
	 sum->result_loopvar=0;
	 retval=1;
      }
   }
#endif
#ifdef RESULT_MASK
   sum->result_loopvar&=RESULT_MASK;
#endif
   return retval;
}
#endif /* HAVE_RESULT_LOOPVAR */
#else
#define RESULT_STACK_START (0)
#endif /* MAX_NUM_LOOPVARS */


#ifdef HAVE_PROGRESS
void print_progress(int sig)
{
   depth_t i;
   struct timeval currtime,resulttime;
   long long depth_time[2],time_factor,est_time_remaining,summation,est_currdepth_time,time_at_currdepth;

  
#if MAX_NUM_LOOPVARS
   printf("sum->num_loopvars=%d\n",sum->num_loopvars);
#endif
   est_time_remaining=est_currdepth_time=0;
   gettimeofday(&currtime,NULL);
   if(sum->stack_depth>1)
   {
      timersub(&currtime,&starttime[sum->stack_depth],&resulttime);
      time_at_currdepth=resulttime.tv_sec;
      timersub(&starttime[sum->stack_depth],&starttime[sum->stack_depth-1],&resulttime);
      depth_time[0]=(resulttime.tv_sec*1000000)+resulttime.tv_usec;
      timersub(&starttime[sum->stack_depth-1],&starttime[sum->stack_depth-2],&resulttime);
      depth_time[1]=(resulttime.tv_sec*1000000)+resulttime.tv_usec;
      time_factor=depth_time[0]/depth_time[1];
      est_currdepth_time=(depth_time[0]*time_factor)/1000000;
      summation=est_currdepth_time;
      est_currdepth_time-=time_at_currdepth;
      for(i=sum->stack_depth;i<=max_stack_depth;i++)
      {
	 est_time_remaining+=summation;
	 summation=summation*time_factor;
      }
      est_time_remaining-=time_at_currdepth;
   }
   timersub(&currtime,&starttime[0],&resulttime);
   printf("curr depth="DEPTH_CHANGE_FORMAT" max_stack_depth="DEPTH_CHANGE_FORMAT
	  " time running=%ld secs est_currdepth_time=%lld secs  est_time_remaining=%lld secs\n",
	  sum->stack_depth,max_stack_depth,resulttime.tv_sec,est_currdepth_time,est_time_remaining);
}
#endif

#ifdef MULTIPLE_RESULTS
#ifdef NUM_ANSWERS
#ifdef SPARSE_ARRAY_INDICES
result_t alloc_result(void)
{ 
   result_t result=(result_t)myalloc("result",sizeof(result_array_t));
   return result;
}
#endif
#else /* NUM_ANSWERS */
result_t alloc_result(int num_answers)
{ 
   result_t result=(result_t)myalloc("result",offsetof(result_array_t,answer[num_answers]));
   result->num_answers=num_answers;
   return result;
}
#endif /* NUM_ANSWERS */
#endif


depth_t get_op_depth(stack_entry *curr)
{
   stack_tag curr_tag=curr->tag;
   switch(curr_tag)
   {
      case arithmetic_operation_tag:
	 return op_depth[curr->val];
#ifdef HAVE_FUNCTIONS
      case function_tag:
	 return NUM_SEQUENCE_DIMENSIONS;
#endif
      default:
	 exit_error("illegal tag in get_depth curr=%p tag="DEPTH_CHANGE_FORMAT"\n",curr,curr_tag);
	 
   }
}



depth_t get_depth_change(stack_entry *curr)
{
   stack_tag curr_tag=curr->tag;
   if(is_number(curr_tag))
      return(1);
   switch(curr_tag)
   {
      case arithmetic_operation_tag:
	 return 1-op_depth[curr->val];
#ifdef HAVE_FUNCTIONS
      case function_tag:
	 return 1-NUM_SEQUENCE_DIMENSIONS;
#endif
      default:
	 exit_error("illegal tag in get_depth_change curr=%p tag="DEPTH_CHANGE_FORMAT"\n",curr,curr_tag);
	 
   }
}





#ifdef HAVE_FACTORIAL_OP
number_t *factorials=NULL;
int max_factorial;
void init_factorials()
{
   int n,pass;
   number_t factorial;

   for(pass=1;pass<=2;pass++)
   {
      number_t factorial=1,oldfactorial=1;
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
	 factorials=myalloc("factorials",sizeof(number_t)*max_factorial);
	 factorials[0]=1;
      }
   }
}
#endif



#ifndef NUM_INTEGER_BITS
int number_to_int_t(number_t *numval,int_t *intval)
{
   if(*numval>MAX_INT_T||*numval<MIN_INT_T) 
      return -1; 
   else
      *intval=(int_t)*numval;
   return 0;
} 
#endif

#ifdef SEQUENCE_HUNTER
result_t *get_array_member(dimension_t *array_indices)
{
#if MULTI_DIMENSIONAL
   dimension_t curr_dimension,curr_index;
   dimension_t member_idx=0;

   for(curr_dimension=0;curr_dimension<NUM_SEQUENCE_DIMENSIONS;curr_dimension++)
   {
      curr_index=array_indices[curr_dimension];
      if(curr_index>=sequence_dimension[curr_dimension]||curr_index<0)
	 exit_error("get_array_member illegal dimension index"
		    DIMENSION_FORMAT" attempted to access "
		    DIMENSION_FORMAT" max="DIMENSION_FORMAT"\n",curr_dimension,curr_index,
		    sequence_dimension[curr_dimension]);
      member_idx+=(sequence_dimension_multiplier[curr_dimension]*curr_index);
   }
   return(&sequence_array[member_idx]);
#else
   if(array_indices[0]>=sequence_dimension[0]||array_indices[0]<0)
	 exit_error("get_array_member illegal dimension index "
		    " attempted to access "
		    DIMENSION_FORMAT" max="DIMENSION_FORMAT"\n",array_indices[0],
		    sequence_dimension[0]);
   return(&sequence_array[array_indices[0]]);
#endif
}


int increment_array_indices(dimension_t seed_idx
#ifdef SPARSE_ARRAY_INDICES
			,int skip_null
#endif
)
{
   int retval;
#ifdef SPARSE_ARRAY_INDICES
   do
   {
#endif
#ifdef MULTI_DIMENSIONAL
      dimension_t curr_dimension;

      for(curr_dimension=0;curr_dimension<NUM_SEQUENCE_DIMENSIONS;curr_dimension++)
      {
	 array_indices[curr_dimension]++;
	 if(array_indices[curr_dimension]>=sequence_dimension[curr_dimension])
	    array_indices[curr_dimension]=seed_idx;
	 else
	    break;
      }
      retval=(curr_dimension>=NUM_SEQUENCE_DIMENSIONS);
#else
      array_indices[0]++;
      if(array_indices[0]>=sequence_dimension[0])
      {
	 array_indices[0]=seed_idx;
	 retval=1;
      }
      else
	 retval=0;
#endif
#ifdef SPARSE_ARRAY_INDICES
   } while(skip_null&&!retval&&!*get_array_member(array_indices));
#endif
   return retval;
}


void init_array_indices(dimension_t seed_idx
#ifdef SPARSE_ARRAY_INDICES
			,int skip_null
#endif
)
{
#if MULTI_DIMENSIONAL
   dimension_t curr_dimension;

   for(curr_dimension=0;curr_dimension<NUM_SEQUENCE_DIMENSIONS;curr_dimension++)
      array_indices[curr_dimension]=seed_idx;
#else
   array_indices[0]=seed_idx;
#endif
 #ifdef SPARSE_ARRAY_INDICES
   if(skip_null&&!*get_array_member(array_indices))
      if(increment_array_indices(seed_idx,TRUE))
	 exit_error("empty sparse array indices\n");
#endif
}




#ifdef HAVE_FUNCTIONS
int cast_to_dimensions(dimension_t *dimensions,dimension_t *max_dimensions,number_t *numbers)
{
#if MULTI_DIMENSIONAL
   dimension_t curr_dimension;


   for(curr_dimension=0;curr_dimension<NUM_SEQUENCE_DIMENSIONS;curr_dimension++,numbers++,max_dimensions++,dimensions++)
#endif
   {
      if(*numbers>=*max_dimensions||*numbers<0)
	 return -1;
      else
	 *dimensions=*numbers;
   }
   return(0);
}
#endif
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

typedef struct
{
      long lo_val;
      long hi_val;
      stack_tag     next_tag;
} number_range_t;

number_range_t number_range[last_number_tag+1]=
{
#if MAX_NUM_LOOPVARS
   {0,MAX_NUM_LOOPVARS,0},
#endif
#ifdef SEQUENCE_HUNTER
   {0,-1,0},
#endif
#ifdef HAVE_CONSTANTS_FILE
   {0,-1,0},
#endif

   {1,0,0},
};

stack_tag first_number;

void init_number(stack_entry *curr)
{
#ifdef SIGNED_OPERATION
   curr->minus=FALSE;
#endif
   curr->tag=first_number;
   curr->val=number_range[first_number].lo_val;
}





int increment_numbers()
{
   int idx,done;
   stack_entry *curr;
#ifdef HAVE_ERROR_MEASUREMENTS
   int have_number=FALSE;
#endif
#if MAX_NUM_LOOPVARS
   do
   {
#endif
      done=FALSE;
      for(idx=SKIP_DEPTH;idx<sum->stack_depth;idx++)
      {
	 curr=&sum->stack[idx];
	 if(is_number(curr->tag))
	 {
#ifdef HAVE_ERROR_MEASUREMENTS
	    have_number=TRUE;
#endif
	    done=FALSE;
#ifdef SIGNED_OPERATION
	    if(curr->minus==FALSE)
	       curr->minus=TRUE;
	    else
#endif
	    {
#ifdef SIGNED_OPERATION
	       curr->minus=FALSE;
#endif
	       curr->val++;
	       if(curr->val>number_range[curr->tag].hi_val)
	       {
		  if(curr->tag>=number_range[curr->tag].next_tag)
		     done=TRUE;
		  curr->tag=number_range[curr->tag].next_tag;
		  curr->val=number_range[curr->tag].lo_val;
		  if(done)
		     continue;
	       }
	    }
	    goto finished;
	 }
      }
     finished:;
      done=(idx>=sum->stack_depth&&(done
#ifdef HAVE_ERROR_MEASUREMENTS
				    ||!have_number
#endif
	       ));
#if MAX_NUM_LOOPVARS
   } while(!done&&!check_all_loopvars_used());
#endif
   return done;
}



#ifdef HAVE_FUNCTIONS
void init_function(stack_entry *curr)
{
#ifdef SIGNED_OPERATION
   curr->minus=FALSE;
#endif
   curr->tag=function_tag;
}
#endif


void init_operation(stack_entry *curr,depth_t depth)
{
#ifdef SIGNED_OPERATION
   curr->minus=FALSE;
#endif
   curr->tag=arithmetic_operation_tag;
   curr->val=first_operator[depth-1];
}


/* counting order for binaries operators */
/* 000111,001011,001101,001110,010011,010101,010110,011001.....,111000 */


int increment_sum_order()
{
   int idx
#ifdef HAVE_FUNCTIONS
      ,num_functions
#endif
#ifdef MAX_NUM_LOOPVARS
      ,num_loopvars
#endif
      ;
   depth_t depth_change,depth;
   stack_entry *curr;
  retry:;
   for(idx=curr_depth1;idx>=SKIP_DEPTH;idx--)
   {
      curr=&sum->stack[idx];
      switch(curr->tag)
      {
#if MAX_NUM_LOOPVARS
	 case loopvar_tag:
#endif
#ifdef SEQUENCE_HUNTER
	 case dimension_tag:
#endif
#ifdef HAVE_CONSTANTS_FILE
	 case constant_tag:
#endif
	 case integer_tag:
	    init_operation(curr,min_operator_depth);
	    goto leave_loop;
	 case arithmetic_operation_tag:
	    depth=op_depth[curr->val];
	    if(depth<max_operator_depth&&idx>depth)
	    {
	       init_operation(curr,depth+1);
	       goto leave_loop;
	    }
#ifdef HAVE_FUNCTIONS
	    if(idx>=NUM_SEQUENCE_DIMENSIONS)
	    {
	       init_function(curr);
	       goto leave_loop;
	    }
	 case function_tag:
#endif
	    init_number(curr);
	    if(idx==SKIP_DEPTH)
	       return 1;
	    break;
      }
   }
  leave_loop:;
#ifdef HAVE_FUNCTIONS
   num_functions=
#endif
#ifdef MAX_NUM_LOOPVARS
      num_loopvars=
#endif
      0;
   depth=(SKIP_DEPTH ? 1:0);
   for(idx=SKIP_DEPTH;idx<sum->stack_depth;idx++)
   {
      curr=&sum->stack[idx];
      depth_change=get_depth_change(curr);
      depth+=depth_change;
      if(depth<1)
	 goto retry;
      switch(curr->tag)
      {
	 case arithmetic_operation_tag:
	    if(depth_change==(min_operator_depth-1)&&idx==depth_change)
		  return 1;
	    break;
#ifdef HAVE_FUNCTIONS
	 case function_tag:
	    num_functions++;
	    break;
#endif
#ifdef MAX_NUM_LOOPVARS
	 case loopvar_tag:
	    num_loopvars++;
	    break;
#endif
      }
   }
      if(depth!=1)
	 goto retry;
#ifdef MAX_NUM_LOOPVARS
      if(num_loopvars<sum->num_loopvars)
	 goto retry;
      if(sum->num_loopvars>1)
      {
	 /* change the loopvars to the lowest valid order */
	 for(idx=SKIP_DEPTH;idx<sum->stack_depth;idx++)
	 {
	    int loopvar_val=curr_num_loopvars1;
	    curr=&sum->stack[idx];
	    if(curr->tag==loopvar_tag)
	       curr->val=loopvar_val--;
	    if(loopvar_val<0)
	       break;
	 }
      }
#endif
   return 0;
}



/* Inintialise the sum to the lowest valid sum */
int init_stack_list()
{
   int idx;

   for(idx=SKIP_DEPTH;idx<sum->stack_depth;idx++)
      init_number(&sum->stack[idx]);
   if(sum->stack_depth>1)
      return increment_sum_order();
   return 0;
}

int increment_operators(depth_t depth)
{
   depth_t idx;
   stack_entry *curr;
   operation last_operation=(depth<max_operator_depth ?
			     first_operator[depth] : num_operators);

   for(idx=depth+SKIP_DEPTH;idx<sum->stack_depth;idx++)
   {
      curr=&sum->stack[idx];
      if(curr->tag==arithmetic_operation_tag&&op_depth[curr->val]==depth)
      {
	 curr->val++;
	 if(curr->val>=last_operation)
	    curr->val=first_operator[depth-1];
	 else
	    break;
      }
   }
   return(idx>=sum->stack_depth);
}



#if defined(HAVE_RATIOED_ERROR_MEASUREMENTS) && (defined(HAVE_BLOCK_DIST_RATIOED_ERROR) || defined(HAVE_DIST_RATIOED_ERROR))
error_t calulate_error_ratio(dimension_t *array_indices)
{
#if  MULTI_DIMENSIONAL
   error_t     retval=0;
   dimension_t curr_dimension,curr_index;
   for(curr_dimension=0;curr_dimension<NUM_SEQUENCE_DIMENSIONS;curr_dimension++)
   {
      curr_index=array_indices[curr_dimension];
#ifdef HAVE_BLOCK_DIST_RATIOED_ERROR
      retval+=curr_index;
#endif
#ifdef HAVE_DIST_RATIOED_ERROR
   retval+=(curr_index*curr_index);
#endif      
   }
#ifdef HAVE_DIST_RATIOED_ERROR
   retval=sqrt(retval);
#endif
   return(retval);
#else
   return(array_indices[0]);
#endif
}
#endif


 
#if !defined(NUM_INTEGER_BITS) && !defined(ERROR_OP)
number_t error1;
#endif
int result_correct(result_t testvals)
{
   int retval;
#ifdef HAVE_ERROR_MEASUREMENTS
   error_t error_pow1
#ifdef HAVE_LMS_ERROR_MEASUREMENTS
	,error_lms
#endif
#ifdef HAVE_POWER1_RATIOED_ERROR_MEASUREMENTS
      ,error_ratioed_pow1
#endif
#ifdef HAVE_LMS_RATIOED_ERROR_MEASUREMENTS
      ,error_ratioed_lms
#endif
      ;
#endif /*  HAVE_ERROR_MEASUREMENTS */

#ifdef MULTIPLE_RESULTS
#ifndef PRINT_ONLY_FIRST_CORRECT_ANSWER
   int retval1=0;
#endif
   int curr_answer,num_answers=
#ifdef NUM_ANSWERS
      NUM_ANSWERS
#else
      testvals->num_answers
#endif
      ;
   number_t *answers=
#if (!defined(NUM_ANSWERS)||defined(SPARSE_ARRAY_INDICES))
      &testvals->answer[0];
#else
   &testvals.answer[0];
#endif
   
   for(curr_answer=0;curr_answer<num_answers;curr_answer++)
#endif /* MULTIPLE_RESULTS */
   {
#ifdef HAVE_ERROR_MEASUREMENTS
      error_pow1 = (error_t)result_stack[0] - (error_t)
#ifdef MULTIPLE_RESULTS
	 *answers++
#else
	 testvals
#endif
	   ;
#ifdef HAVE_POWER1_RATIOED_ERROR_MEASUREMENTS
	   error_ratioed_pow1 = error_pow1/calculate_error_ratio(array_indices);
#endif
#ifdef HAVE_LMS_ERROR_MEASUREMENTS
	error_lms=(error_pow1*error_pow1);
#endif
#ifdef HAVE_POWER1_RATIOED_ERROR_MEASUREMENTS
	   error_ratioed_lms = error_lms/calculate_error_ratio(array_indices);
#endif
#ifdef HAVE_LMS_ERROR
	   error_val[lms_error]+=error_lms;
#endif
#ifdef HAVE_ABS_ERROR
	   error_val[abs_error]+=(error_pow1>=0 ? error_pow1 : -error_pow1);
#endif

#ifdef HAVE_GT_ERROR_MEASUREMENTS
	   if(error_pow1>=0)
	   {
#ifdef HAVE_GT_ERROR
	      error_val[gt_error]+=error_pow1;
#endif
#ifdef HAVE_LMS_GT_RATIOED_ERROR
	      error_val[lms_gt_error]+=error_lms;
#endif
#ifdef HAVE_GT_RATIOED_ERROR
	      error_val[gt_ratioed_error]+=error_ratioed_pow1;
#endif
	   }
#endif
#ifdef HAVE_LT_ERROR_MEASUREMENTS
	   if(error_pow1<=0)
	   {
#ifdef HAVE_LT_ERROR
	      error_val[lt_error]-=error_pow1;
#endif
#ifdef HAVE_LMS_LT_RATIOED_ERROR
	      error_val[lms_lt_error]+=error_lms;
#endif
#ifdef HAVE_LT_RATIOED_ERROR
	      error_val[lt_ratioed_error]-=error_ratioed_pow1;
#endif
	   }
#endif 
#endif

#ifdef NUM_INTEGER_BITS
      retval = (result_stack[0]==
#ifdef MULTIPLE_RESULTS
	 *answers++
#else
	 testvals
#endif
	 ); 
#else /* NUM_INTEGER_BITS */
      {

#ifdef ERROR_OP
      retval = (ERROR_OP(result_stack[0])==
#ifdef MULTIPLE_RESULTS
	 *answers++
#else
	 testvals
#endif
	 );
#else   /* ERROR_OP */
      if(result_stack[0]==0)
	error1=
#ifdef MULTIPLE_RESULTS
	  (*answers++);
#else
	  testvals;
#endif
	else
	  error1=(
#ifdef MULTIPLE_RESULTS
		  (*answers++)
#else
		  testvals
#endif
		  /result_stack[0])-1.0;
	 if(error1<0.0)
	    error1=-error1;
#if 0
	 retval = (error1<error_tolerance);
#else
	 if (error1<error_tolerance)
	   retval=TRUE;
	 else 
	   retval=FALSE;
#endif
#endif /* ERROR_OP */
      }
#endif
#ifdef MULTIPLE_RESULTS
   retvals[curr_answer]+=retval;
#ifdef PRINT_ONLY_FIRST_CORRECT_ANSWER
   if(retval)
      break;
#else
   retval1|=retval;
#endif
#endif
   }
#if defined(MULTIPLE_RESULTS) && !defined(PRINT_ONLY_FIRST_CORRECT_ANSWER)
   return retval1;
#else
   return retval;
#endif
}



#ifdef SEQUENCE_HUNTER
#ifdef HAVE_FUNCTIONS
#ifdef SIGNED_OPERATION
int increment_functions()
{
   int idx;
   stack_entry *curr;

   for(idx=SKIP_DEPTH;idx<sum->stack_depth;idx++)
   {
      curr=&sum->stack[idx];
      if(curr->tag==function_tag)
      {
	 if(curr->minus==FALSE)
	 {
	    curr->minus=TRUE;
	    break;
	 }
	 else
	    curr->minus=FALSE;
      }
   }
   return(idx>=sum->stack_depth); 
}
#endif
#endif
#else
int check_sum()
{
   int idx1,idx2;   
   stack_entry *curr;
   fundamental_constant *curr_const;

   if(result_stack[0]!=0)
   {
      for(idx1=0;idx1<num_constants;idx1++)
      {
	 for(idx2=0;idx2<sum->stack_depth;idx2++)
	 {
	    curr=&sum->stack[idx2];
	    if(curr->tag==constant_tag&&curr->val==idx1)
	       goto Skip;
	 }
	 curr_const=fundamental_list[idx1];
	 if(result_correct(curr_const->value))
	 {
	    printf("Found match "
#ifndef NUM_INTEGER_BITS
		   "error="NUMBER_FORMAT
#endif
		   " fundamental constant name=%s value="NUMBER_FORMAT"\n",
#ifndef NUM_INTEGER_BITS
		   error1,
#endif
		   curr_const->name,curr_const->value);
	    return(idx1);
	 }
	Skip:;
      }
   }
   return(-1);
}
#endif



int compare_stack_entries(const void *c1, const void *c2)
{
   return(((stack_entry *)c1)->tag-((stack_entry *)c1)->tag);
}



int loop_operators()
{
   int done=TRUE;
   depth_t depth;

   for(depth=min_operator_depth;depth<=max_operator_depth&&done;depth++)
      done=increment_operators(depth);
   return done;
}

number_t *curr_result_ptr;
int sum_switch(stack_entry *curr)
{
#ifdef HAVE_FUNCTIONS
      number_t *function_result_ptr
#endif
      ;

   switch(curr->tag)
   {
#ifdef MAX_NUM_LOOPVARS
      case loopvar_tag:
#ifdef SIGNED_OPERATION
	 if(curr->minus)
	    *curr_result_ptr++=-loopvar[curr->val];
	 else
#endif
	    *curr_result_ptr++=loopvar[curr->val];
	 break;
#endif
#ifdef SEQUENCE_HUNTER
#ifdef HAVE_FUNCTIONS
      case function_tag:
	 function_result_ptr=(curr_result_ptr-NUM_SEQUENCE_DIMENSIONS);
	 if(cast_to_dimensions(temp_dimensions,array_indices,function_result_ptr))
	 {
	    return TRUE;
	 }
	 *function_result_ptr=(
#ifdef SIGNED_OPERATION
	    curr->minus ? -*get_array_member(temp_dimensions) :
#endif
	    *get_array_member(temp_dimensions));
	 curr_result_ptr-=(NUM_SEQUENCE_DIMENSIONS-1);
	 break;
#endif
      case dimension_tag: 
#ifdef SIGNED_OPERATION
	 if(curr->minus)
	    *curr_result_ptr++=-array_indices[curr->val];
	 else
#endif
	    *curr_result_ptr++=array_indices[curr->val];
	 break;
#endif
#ifdef HAVE_CONSTANTS_FILE
      case constant_tag:
#ifdef SIGNED_OPERATION
	 if(curr->minus)
	    *curr_result_ptr++=-fundamental_list[curr->val]->value;
	 else
#endif
	    *curr_result_ptr++=fundamental_list[curr->val]->value;
	 break;
#endif

      case integer_tag:
#ifdef SIGNED_OPERATION
	 if(curr->minus)
	    *curr_result_ptr++=-(number_t)curr->val;
	 else
#endif
	    *curr_result_ptr++=(number_t)curr->val;
	 break;
      case arithmetic_operation_tag:
	 if(do_sum(&curr_result_ptr,curr))
	    return TRUE;
	 break;
   }
#ifdef RESULT_MASK
   *(curr_result_ptr-1)&=RESULT_MASK;
#endif
   return FALSE;
}

void sum_correct_func(calculate_sum_result *retval)
{
      if(!retval->aborted)
      {
#ifdef SEQUENCE_HUNTER
	 if(!result_correct(*get_array_member(array_indices)))
	    retval->sum_correct=FALSE;
	 
#else
	 if(check_sum()==-1)
	    retval->sum_correct=FALSE;
	 else
	 {
	    printf("result="NUMBER_FORMAT"\n",result_stack[0]);
	 }
#endif
      }
}

calculate_sum_result calculate_sum(sum_t *sum,calculate_sum_func_t sum_func)
{

   int idx;
   
   calculate_sum_result retval;
   retval.sum_correct=TRUE;
#ifdef SEQUENCE_HUNTER
#ifdef MULTIPLE_RESULTS
   memset(retvals,0,MAX_NUM_RESULTS*sizeof(*retvals));
#endif
   init_array_indices(
#ifdef HAVE_FUNCTIONS 
      sum->seed
#else
      MIN_SEED
#endif
#ifdef SPARSE_ARRAY_INDICES
      ,TRUE
#endif
      );
   do
   {
#endif /* SEQUENCE_HUNTER */
#if MAX_NUM_LOOPVARS
      int inc_lv_retval;
#ifdef HAVE_RESULT_LOOPVAR
      result_stack[0]=sum->result_loopvar;
#else
      result_stack[0]=0;
#endif   
      init_loopvars();
      do
      {
#endif /* MAX_NUM_LOOPVARS */
	 
	 curr_result_ptr=&result_stack[RESULT_STACK_START];
	 for(idx=0;idx<sum->stack_depth;idx++)
	 {
	    retval.aborted=sum_switch(&sum->stack[idx]);
	    if(retval.aborted)
	       goto skip;
	 }
	 if(curr_result_ptr!=&result_stack[1+RESULT_STACK_START])
	 {
	    fprintf(stderr,"Sum below is illegal curr_result_ptr(%p)"
		    "!=&result_stack[1+RESULT_STACK_START](%p)\n",
		    curr_result_ptr,&result_stack[1+RESULT_STACK_START]);
	    print_sum(sum);
	    exit(-1); 
	 }
#if MAX_NUM_LOOPVARS
	 inc_lv_retval=increment_loopvars();
	 if(inc_lv_retval==-1)
	    retval.aborted=TRUE;
      } while(!inc_lv_retval);
#endif
     skip:;
      sum_func(&retval);
#ifdef SEQUENCE_HUNTER
   } while(!retval.aborted&&(retval.sum_correct
#ifdef HAVE_ERROR_MEASUREMENTS
		      ||error_sums_good()
#endif
	      )
	   
	   &&!increment_array_indices(
#ifdef HAVE_FUNCTIONS
	      sum->seed
#else
	      MIN_SEED
#endif
#ifdef SPARSE_ARRAY_INDICES
	      ,TRUE
#endif
	      ));
#endif /* SEQUENCE_HUNTER */
   return retval;
}



void process_fundamentals()
{

#if MAX_NUM_LOOPVARS 
   for(sum->num_loopvars=MIN_NUM_LOOPVARS;sum->num_loopvars<=MAX_NUM_LOOPVARS;sum->num_loopvars++)
   {
      curr_num_loopvars1=sum->num_loopvars-1;
      number_range[loopvar_tag].hi_val=curr_num_loopvars1;
      number_range[last_number].next_tag=(sum->num_loopvars ?  loopvar_tag : number_range[loopvar_tag].next_tag);
#endif
      for(sum->stack_depth=
#if MAX_NUM_LOOPVARS
	     MAX(sum->num_loopvars,1+SKIP_DEPTH)
#else
	     1+SKIP_DEPTH
#endif
	     ;sum->stack_depth<=MAX_STACK_DEPTH;

#if min_operator_depth==max_operator_depth==2 && (!defined(HAVE_FUNCTIONS) || NUM_SEQUENCE_DIMENSIONS==2)
	  sum->stack_depth+=2
#else
	     sum->stack_depth++
#endif

	 )
      {
	 curr_depth1=sum->stack_depth-1;
#ifdef HAVE_PROGRESS
	 gettimeofday(&starttime[sum->stack_depth],NULL);
#endif
#if defined(HAVE_FUNCTIONS)
	 for(sum->seed=min_seed;sum->seed<=max_seed;sum->seed++)
#endif
	 {
	    if(init_stack_list())
	       goto skip_depth;
	    do
	    {
#if defined(HAVE_FUNCTIONS) && defined(SIGNED_OPERATION)
	       do
	       {
#endif
		  do
		  {
		     
		     do
		     {   
#ifdef HAVE_ERROR_MEASUREMENTS
			/* This I believe works for floating point values as well */
			memset(&error_val,0,sizeof(error_t)*NUM_ERROR_MEASUREMENTS);
#endif
#if MAX_NUM_LOOPVARS			
			init_looptype_indices();
			do
			{
			   init_loopvar_operators();
			   do
			   {

#ifdef HAVE_RESULT_LOOPVAR
			      init_result_loopvar();
			      do
			      {
#endif
#endif /* MAX_NUM_LOOPVARS */
				 calculate_sum_result result=calculate_sum(sum,sum_correct_func);
				 if(!result.aborted)
				 {
#ifdef HAVE_ERROR_MEASUREMENTS
				    int idx;
				    for(idx=0;idx<NUM_ERROR_MEASUREMENTS;idx++)
				       add_sum_to_list(idx);
#endif
				    if(result.sum_correct)
				       print_sum(sum);
				 }
#if MAX_NUM_LOOPVARS
#ifdef HAVE_RESULT_LOOPVAR
			      } while(!increment_result_loopvar());
#endif
			   } while(!increment_loopvar_operators());
			}while(!increment_looptype_indices());
#endif
		     } while(!increment_numbers());
		  } while(!loop_operators());
#if defined(HAVE_FUNCTIONS) && defined(SIGNED_OPERATION)
	       } while(!increment_functions());
#endif
	    }while(!increment_sum_order());
	 }
	skip_depth:;
      }
#if MAX_NUM_LOOPVARS
   }
#endif
}

#ifdef HAVE_ERROR_MEASUREMENTS
void process_fundamentals_have_error_measurements()
{
   for(next_refinement_depth=refinement_depth;next_refinement_depth<=max_stack_depth;next_refinement_depth+=refinement_depth-1)
   {
      if(next_refinement_depth==refinement_depth)
	 process_fundamentals();
      else
      {
	  int idx;
	
	  for(idx=0;idx<NUM_ERROR_MEASUREMENTS;idx++)
	  {
	     struct list_head *curr_error_list=&error_list[idx];
	     error_list_element *element;
	     list_for_each(((struct list_head *)element),curr_error_list)
	     {
		if(!element->processed)
		{
		   element->processed=TRUE;
		   memcpy(sum,&element->sum,offsetof(sum_t,stack[element->sum.stack_depth]));
		   skip_depth=element->sum.stack_depth;
		   process_fundamentals();
		}
	     }
	  }
      }
   }
   print_error_measurements();
}

#define PROCESS_FUNDAMENTALS() process_fundamentals_have_error_measurements()
#else
#define PROCESS_FUNDAMENTALS() process_fundamentals()
#endif

#ifdef HAVE_ERROR_MEASUREMENTS
static void signal_print_error_measurements(int signal)
{
   printf("Intermediate results\n");
   print_error_measurements();
}
#endif

int main(int argc,char *argv[])
{
   stack_tag curr_number,next_number;
   int idx,c,line;
#if defined(HAVE_CONSTANTS_FILE) || defined(SEQUENCE_HUNTER)
   FILE *fstream;
   char numbuff[40];
   int numassigned; 
#endif
   hi_int=0;
#ifdef SEQUENCE_HUNTER
   sequence_array=NULL;
#endif
  
   if(argc<5)
      goto error;
#ifdef HAVE_FACTORIAL_OP
   init_factorials();
#endif
#ifdef HAVE_ERROR_MEASUREMENTS
   for(idx=0;idx<NUM_ERROR_MEASUREMENTS;idx++)
   {
      num_error_list_elements[idx]=0;
      INIT_LIST_HEAD(&error_list[idx]);
   }
#endif
   for(;;)
   {
      c=getopt(argc,argv,"h:m:a:"
#ifdef HAVE_CONSTANTS_FILE
	       "c:"
#endif
#ifdef SEQUENCE_HUNTER
	       "f:i:s:l:"
#endif
#ifndef NUM_INTEGER_BITS
	       "e:"
#endif
#ifdef HAVE_ERROR_MEASUREMENTS
	 "r:"
#endif
	 );
      if (c == -1)
	 break;
      if(optind==-1||optarg==NULL)
	 goto error;
      switch (c)
      {
	 case 'h':
	    hi_int=atoi(optarg);
	    if(hi_int<=0)
	       goto error;
	    break;
#ifdef HAVE_CONSTANTS_FILE
	 case 'c':
	    fstream=fopen(optarg,"r");
	    if(fstream==NULL)
	    {
	       perror("");
	       exit_error("An error occured opening %s\n");
	    }
	    idx=0;
	    for(;;idx++)
	    {
	       fundamental_constant *new_entry,*prev_entry;
	       char type,name[1024];
	       number_t value;
	    
	       /* fscanf on linux doesn't handle doubles */
	       numassigned=fscanf(fstream,"%c %1023s %39s\n",&type,&name,&numbuff);
	       value=ASCII_TO_NUM(numbuff);
	       if(numassigned==3)
	       {
	       
		  new_entry=(fundamental_constant *)malloc(offsetof(fundamental_constant,name[strlen(name)+1]));
		  if(new_entry==NULL)
		     exit_error("Ran out of memory allocating a fundamental_constant %s on line %d",name,idx);
		  new_entry->next=NULL;
		  strcpy(new_entry->name,name);
		  new_entry->type=type;
		  new_entry->value=value;
		  if(idx==0)
		     head=new_entry;
		  else
		     prev_entry->next=new_entry;
		  prev_entry=new_entry;
	       }
	       else
	       {
		  if(numassigned>0)
		     exit_error("syntax error on line %d or %s\n",idx+1,optarg);
		  break;
	       }
	    }
	    fclose(fstream);
	    if(idx==0)
	       exit_error("No entries found in file %s\n",optarg);
	    num_constants=idx;
	    fundamental_list=(fundamental_constant **)myalloc("fundamental_list",num_constants*sizeof(fundamental_constant *));
	    if(fundamental_list)
	    {
	       fundamental_constant *curr_entry=head;
	    
	       for(idx=0;idx<num_constants;idx++)
	       {
		  fundamental_list[idx]=curr_entry;
		  if(curr_entry->next)
		     curr_entry=curr_entry->next;
	       }
	    }
	    else
	       exit_error("Ran out of memory allocating indexed_head\n");
	    break;
#endif
	 case 'a':
	    max_num_answers=atoi(optarg);
	    break;
	 case 'm':
	    max_stack_depth=atoi(optarg);
	    if(max_stack_depth<1)
	       max_stack_depth=1;  
#ifndef HAVE_UNARY_OPERATORS
	    max_stack_depth|=1;
#endif
	    sum=(sum_t *)myalloc("sum",offsetof(sum_t,stack[max_stack_depth]));
#ifdef HAVE_PRINT_SUM_INFIX
	    tree_members=(struct infix_tree **)myalloc("tree_members",sizeof(struct infix_tree *)*max_stack_depth);
#endif
  	    result_stack=(number_t *)myalloc("result_stack",sizeof(number_t)*
					     ((max_stack_depth>>1)
#if MAX_NUM_LOOPVARS
						+1 
#endif
					       ));
	    break;

#ifdef SEQUENCE_HUNTER
#ifndef MULTIPLE_RESULTS
	 case 'f':
	 case 'i':
#ifndef NUM_SEQUENCE_DIMENSIONS
	    NUM_SEQUENCE_DIMENSIONS=atoi(optarg);
#endif
#endif
	 case 's':
	 {
	    int curropt=optind
#ifdef NUM_SEQUENCE_DIMENSIONS
	       -1
#else
	       -((c=='i'||c=='f') ? 0:1)
#endif
	       ;
#ifdef HAVE_FUNCTIONS
	    min_seed=atoi(argv[curropt++]);
	    max_seed=atoi(argv[curropt++]);
	    if(max_seed<0||NUM_SEQUENCE_DIMENSIONS<1||min_seed>max_seed)
	       goto error;
#endif
	    sequence_dimension=myalloc("sequence_dimension",NUM_SEQUENCE_DIMENSIONS*sizeof(dimension_t));
	    array_indices=myalloc("array_indices",NUM_SEQUENCE_DIMENSIONS*sizeof(dimension_t));
	    temp_dimensions=myalloc("temp_dimensions",NUM_SEQUENCE_DIMENSIONS*sizeof(dimension_t));
	    sequence_dimension_multiplier=myalloc("sequence_dimension_multiplier",NUM_SEQUENCE_DIMENSIONS*sizeof(dimension_t));
	    sequence_array_size=1;
	    for(idx=0;idx<NUM_SEQUENCE_DIMENSIONS;idx++)
	    {
	       int temp_dimension=atoi(argv[curropt++]);
	       if(temp_dimension<=0)
		  exit_error("Illegal sequence_dimension idx=%d dimension_val=%d\n",idx,temp_dimension);
	       sequence_dimension[idx]=temp_dimension;
	       sequence_dimension_multiplier[idx]=sequence_array_size;
	       sequence_array_size*=temp_dimension;
	       
	    }
	    sequence_array=myalloc("sequence_array",sizeof(result_t)*sequence_array_size);
#if defined(SEED) && defined(SPARSE_ARRAY_INDICES)
	    memset(sequence_array,0,sizeof(result_t)*SEED);
#endif
	    init_array_indices(MIN_SEED
#ifdef SPARSE_ARRAY_INDICES
			       ,FALSE
#endif
	       );
#if 0 /* looks like old code */
	    idx=optind+2+NUM_SEQUENCE_DIMENSIONS;
#endif
#if defined(MULTIPLE_RESULTS) && !defined(NUM_ANSWERS)
	    MAX_NUM_RESULTS=0;
#endif
#ifndef MULTIPLE_RESULTS
	    if(c=='f')
	    {
	       line=1;
	       fstream=fopen(argv[curropt++],"r");
	       if(fstream==NULL)
	       {
		  perror("");
		  exit_error("An error occured opening %s\n");
	       }
	    }
#endif
	    do
	    {
	       int retval;
	       result_t result;
#ifndef MULTIPLE_RESULTS
	       if(c=='s')
#endif
	       {
		  if(!sequence_func(&result,array_indices))
		  {
		     *get_array_member(array_indices)=result;
#if defined(MULTIPLE_RESULTS) && !defined(NUM_ANSWERS)
#ifdef SPARSE_ARRAY_INDICES
		     if(!result)
			continue;
#endif
		     if(result->num_answers<=0)
			exit_error("Illegal number of answers=%d\n",result->num_answers);
		     if(result->num_answers>MAX_NUM_RESULTS)
			MAX_NUM_RESULTS=result->num_answers;
#endif
		  }
		  else
		     exit_error("illegal sequence attempt\n");
	       }
#ifndef MULTIPLE_RESULTS
	       else if(c=='i')
		  *get_array_member(array_indices)=ASCII_TO_NUM(argv[curropt++]);
	       else if(c=='f')
	       {
		  numassigned=fscanf(fstream,"%39s\n",&numbuff);
		  if(numassigned==1)
		     *get_array_member(array_indices)=ASCII_TO_NUM(numbuff);
		  else
		     exit_error("syntax error in file %s or %s\n",optarg,line);
		  line++;
	       }
#endif
	    } while(!increment_array_indices(MIN_SEED
#ifdef SPARSE_ARRAY_INDICES
			       ,FALSE
#endif
		       ));
#ifndef MULTIPLE_RESULTS
	    if(c=='f')
	       fclose(fstream);
#endif
#ifdef MULTIPLE_RESULTS
	    retvals=myalloc("retvals",sizeof(*retvals)*MAX_NUM_RESULTS);
#endif
	 }
	 break;
#endif
#if !defined(NUM_INTEGER_BITS) && !defined(ERROR_OP)
	 case 'e':
	    error_tolerance=atof(optarg);
	    if(error_tolerance<0.0||error_tolerance>1.0)
	       exit_error("illegal tolerance specification must be between 0 & 1.\n");
	    break;
#endif
#ifdef HAVE_ERROR_MEASUREMENTS
	 case 'r':
	    refinement_depth=atoi(optarg);
	    if(refinement_depth<=0||refinement_depth>=100)
	       exit_error("illegal refinement depth it must be between 0 & 100.\n");
	    break;
#endif
	 default:
	    goto error;
      }
   }
   if(
#ifdef SEQUENCE_HUNTER
      !sequence_dimension
#ifdef HAVE_CONSTANTS_FILE
      &&
#endif
#endif
#ifdef HAVE_CONSTANTS_FILE
      !fundamental_list
#endif 
      )
      goto error;
#ifdef HAVE_PROGRESS
   starttime=myalloc("starttime",(max_stack_depth+1)*sizeof(*starttime));
   gettimeofday(&starttime[0],NULL);
   signal(SIGUSR1,print_progress);
#endif
#ifdef HAVE_ERROR_MEASUREMENTS
   signal(SIGUSR2,signal_print_error_measurements);
#endif
#ifdef SEQUENCE_HUNTER
   number_range[dimension_tag].hi_val=NUM_SEQUENCE_DIMENSIONS-1;
#endif
#ifdef HAVE_CONSTANTS_FILE
   if(fundamental_list)
       number_range[constant_tag].hi_val=num_constants-1;
  #endif
    number_range[integer_tag].hi_val=hi_int;
   first_number=first_tag;
   while(number_range[first_number].lo_val>number_range[first_number].hi_val)
      first_number++;
   curr_number=first_number;
   next_number=first_number+1;
   while(next_number<=last_number_tag)
   {
      if(number_range[next_number].lo_val<=number_range[next_number].hi_val)
      {
	 number_range[curr_number].next_tag=next_number;
	 curr_number=next_number;
      }
      next_number++;
   }
   number_range[curr_number].next_tag=first_number;
#if MAX_NUM_LOOPVARS
   last_number=curr_number;
#endif
   PROCESS_FUNDAMENTALS();
   return 0;
  error:;
   exit_error("Usage\n"
	      "=====\n"
	      "%s -h hi_int -m max_stack_depth"
	      "<-a max_num_answers>"
#ifdef HAVE_CONSTANTS_FILE
	      " <-c constants file>"
#endif
#ifdef SEQUENCE_HUNTER
	      " <<-s "
#ifdef HAVE_FUNCTIONS
	      "min_seed max_seed"
#endif
	      " dimensions > "
#ifndef MULTIPLE_RESULTS
	      "<-i "
#ifndef NUM_SEQUENCE_DIMENSIONS
	      "num_sequence_dimensions "
#endif
#ifdef HAVE_FUNCTIONS
	      "min_seed max_seed "
#endif
	      "dimensions sequence_values>> "
#ifndef MULTIPLE_RESULTS
	      "<-f "
#ifndef NUM_SEQUENCE_DIMENSIONS
	      "num_sequence_dimensions "
#endif
#ifdef HAVE_FUNCTIONS
	      "min_seed max_seed "
#endif
	      "dimensions sequence_file>>"
#endif
#endif
#endif /* MULTIPLE_RESULTS */
#if !defined(NUM_INTEGER_BITS) && !defined(ERROR_OP)
	      " <-e error_tolerance >"
#endif
#ifdef  HAVE_ERROR_MEASUREMENTS
	      " <-r refinement depth (default 8)>"
#endif 
	  
	      "\n"
#ifdef NUM_SEQUENCE_DIMENSIONS
	      "NUM_SEQUENCE_DIMENSIONS is defined to be %d\n"

#endif
	      ,argv[0]
#ifdef NUM_SEQUENCE_DIMENSIONS
	      ,NUM_SEQUENCE_DIMENSIONS

#endif
      );
   return -1;
}

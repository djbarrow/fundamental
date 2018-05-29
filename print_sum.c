/*
  This file is part of fundamental a brute force searcher
  for relationships between constants & formulae for sequences.
  Copyright (C) 2004  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

  It is licensed under GPL v2.1.
*/
#include "fundamental.h"
#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <alloca.h>
#include <stdlib.h>

char *operator_str[num_operators+1]=
  {
#ifdef HAVE_FACTORIAL_OP
    "!",
#endif
#ifdef HAVE_ABS_OP
    "abs",
#endif
#ifdef HAVE_FLOOR_OP
    "floor",
#endif
#ifdef HAVE_CEIL_OP
    "ceil",
#endif
#ifdef HAVE_RINT_OP
    "rint",
#endif
#ifdef HAVE_SIN_OP
    "sin",
#endif
#ifdef HAVE_COS_OP
    "cos",
#endif
#ifdef HAVE_TAN_OP
    "tan",
#endif
#ifdef HAVE_ONES_COMPLIMENT_OP
    "compliment",
#endif
#ifdef HAVE_NOT_OP
    "not",
#endif
#ifdef HAVE_ADDITION_OP
    "+",
#endif
#ifdef HAVE_SUBTRACTION_OP
    "-",
#endif
#ifdef HAVE_MULTIPLY_OP
    "*",
#endif
#ifdef HAVE_DIVIDE_OP
    "/",
#endif
#ifdef HAVE_POWER_OP
    "power",
#endif
#ifdef HAVE_MODULO_OP
    "%",
#endif
#ifdef HAVE_AND_OP
    "and",
#endif
#ifdef HAVE_OR_OP
    "or",
#endif
#ifdef HAVE_XOR_OP
    "xor",
#endif
#ifdef HAVE_LOG_SHIFT_OP
    "log_shift",
#endif
#ifdef HAVE_LOG_RSHIFT_OP
    "log_rshift",
#endif
#ifdef HAVE_LOG_LSHIFT_OP
    "log_lshift",
#endif
#ifdef HAVE_ARITH_SHIFT
    "arith_shift",
#endif
#ifdef HAVE_ROTATE_OP
    "rotate",
#endif
#ifdef HAVE_LROTATE_OP
    "lrotate",
#endif
#ifdef HAVE_RROTATE_OP
    "rrotate",
#endif
#ifdef HAVE_MASK_OP
    "mask",
#endif
#ifdef HAVE_GETBIT_OP
    "getbit",
#endif
#ifdef HAVE_LN_OP
    "ln",
#endif
#ifdef HAVE_GCD_OP
    "gcd",
#endif
#ifdef HAVE_EQ_COND
    "==",
#endif
#ifdef HAVE_GT_COND
    ">",
#endif
#ifdef HAVE_GE_COND
    ">=",
#endif
#ifdef HAVE_LT_COND
    "<",
#endif
#ifdef HAVE_LE_COND
    "<=",
#endif
#ifdef HAVE_AND_COND
    "&&",
#endif
#ifdef HAVE_OR_COND
    "||",
#endif
#ifdef HAVE_MIN_OP
    "min",
#endif
#ifdef HAVE_MAX_OP
    "max",
#endif
    (char *)0xdeadbeef,
  };


#ifdef MAX_NUM_LOOPVARS
void print_tabs(int num_tabs)
{
  int idx;

  for(idx=0;idx<num_tabs;idx++)
    printf("  ");
}
#endif


#if defined(MULTIPLE_RESULTS) || defined(HAVE_FUNCTIONS) || MAX_NUM_LOOPVARS
void print_sum_preamble(sum_t *sum)
{
#ifdef MULTIPLE_RESULTS
  dimension_t curr_result;
#endif
#ifdef HAVE_FUNCTIONS
  printf("seed=%d ",sum->seed);
#endif
#ifdef MULTIPLE_RESULTS
  for(curr_result=0;curr_result<MAX_NUM_RESULTS;curr_result++)
    if(retvals[curr_result]==sequence_array_size)
      printf("result=%d ",curr_result);
#endif
  printf("\n");
#if MAX_NUM_LOOPVARS
#ifdef HAVE_RESULT_LOOPVAR
  printf("cum_result="NUMBER_FORMAT"\n",sum->result_loopvar);
#else 
  printf("cum_result=0\n");
#endif
  {
    int idx;
    looptype_t *curr_looptype;
    for(idx=0;idx<sum->num_loopvars;idx++)
      {
	curr_looptype=&looptypes[sum->looptype_idx[idx]];
	print_tabs(idx);
	printf("for lv[%d]="LOOPVAR_FORMAT" to "LOOPVAR_FORMAT" lv[%d]%s\n",
	       idx,curr_looptype->initial_val,curr_looptype->final_val,
	       idx,curr_looptype->loopstr);     
      }
  }
#endif
}
#endif


#ifdef HAVE_PRINT_SUM_INFIX
struct infix_tree  **tree_members=NULL;
#ifdef HAVE_CONSTANTS_FILE
int pass;
#endif
void recurse_sum_infix(depth_t rec_depth,sum_t *sum)
{
  stack_entry *curr=&sum->stack[rec_depth];
  struct infix_tree *curr_tree_member=tree_members[rec_depth];
  depth_t curr_child;
  operation op;
#ifdef SIGNED_OPERATION
  if(curr->minus)
    printf("-");
#endif
   
  switch(curr->tag)
    {
#ifdef SEQUENCE_HUNTER
#ifdef HAVE_FUNCTIONS
    case  function_tag:
      printf("f");
      goto print_comma_seperated;
      break;
#endif
    case dimension_tag:
      printf("n["DIMENSION_FORMAT"]",curr->val);
      break;
#endif
#ifdef HAVE_CONSTANTS_FILE
    case constant_tag:
      if(pass)
	printf(NUMBER_FORMAT" ",fundamental_list[curr->val]->value);
      else
	printf("%s",fundamental_list[curr->val]->name);
      break;
#endif
#if MAX_NUM_LOOPVARS
    case loopvar_tag:
      printf("lv["LOOPVAR_FORMAT"]",curr->val);
      break; 
#endif
    case integer_tag:
      printf(STACKVAL_FORMAT,curr->val);
      break;
    case arithmetic_operation_tag:
      op=curr->val;
#ifdef HAVE_UNARY_OPERATORS
      if(curr_tree_member->depth==1)
	{
	  switch(op)
	    {
#ifdef HAVE_FACTORIAL_OP
	    case factorial_op:
	      printf("(");
	      recurse_sum_infix(curr_tree_member->child[0],sum);
	      printf(")%s",operator_str[curr->val]);
	      break;
#endif
	    default:
	      printf("%s(",operator_str[curr->val]);
	      recurse_sum_infix(curr_tree_member->child[0],sum);
	      printf(")");
	      break;
	    }
	}
      else
#endif
#ifdef HAVE_BINARY_OPERATORS
	{
	  switch(op)
	    {
#ifdef HAVE_ADDITION_OP
	    case addition_op:
#endif
#ifdef HAVE_SUBTRACTION_OP
	    case subtraction_op:
#endif
#ifdef HAVE_MULTIPLY_OP
	    case multiply_op:
#endif
#ifdef HAVE_DIVIDE_OP
	    case divide_op:
#endif
#ifdef HAVE_MODULO_OP
	    case modulo_op:
#endif
#ifdef HAVE_AND_OP
	    case and_op:
#endif
#ifdef HAVE_OR_OP
	    case or_op:
#endif
#ifdef HAVE_XOR_OP
	    case xor_op:
#endif
#ifdef HAVE_LOG_SHIFT_OP
	    case log_shift_op:
#endif
#ifdef HAVE_LOG_RSHIFT_OP
	    case log_rshift_op:
#endif
#ifdef HAVE_LOG_LSHIFT_OP
	    case log_lshift_op:
#endif
#ifdef HAVE_ARITH_SHIFT_OP
	    case arith_shift_op:
#endif
#ifdef HAVE_ROTATE_OP
	    case rotate_op:
#endif
#ifdef HAVE_LROTATE_OP
	    case lrotate_op:
#endif
#ifdef HAVE_RROTATE_OP
	    case rrotate_op:
#endif
	      printf("(");
	      recurse_sum_infix(curr_tree_member->child[0],sum);
#if defined(ARITHMETIC_OPERATORS)&&defined(SIGNED_OPERATION)
	      if(op!=addition||!stack[curr_tree_member->child[1]].minus)
#endif
		printf(" %s ",operator_str[curr->val]);
	      recurse_sum_infix(curr_tree_member->child[1],sum);
	      printf(")"); 
	      break;
		  
	    default:
	      printf("%s",operator_str[curr->val]);
	      goto print_comma_seperated;
	    }
	}
#endif
      break;
    }
  return;
 print_comma_seperated:
  printf("(");
  for(curr_child=0;curr_child<curr_tree_member->depth;curr_child++)
    {
      recurse_sum_infix(curr_tree_member->child[curr_child],sum);
      if(curr_child<curr_tree_member->depth-1)
	printf(",");
    }
  printf(")");
  return;
}
  
void print_sum_infix(sum_t *sum)
{
  struct infix_tree  *curr_tree_member; 
  int idx,idx1;
  depth_t depth;

  stack_entry *curr;
  memset(tree_members,0,sizeof(struct infix_tree *)*sum->stack_depth);
  for(idx=0;idx<sum->stack_depth;idx++)
    {
      curr=&sum->stack[idx];
      depth=(is_operator(curr->tag) ? get_op_depth(curr):0);
      curr_tree_member=tree_members[idx]=alloca(offsetof(struct infix_tree,child[depth]));
      curr_tree_member->has_parent=FALSE;
      curr_tree_member->depth=depth;
      for(idx1=idx-1;idx1>=0&&depth>0;idx1--)
	{
	  if(!tree_members[idx1]->has_parent)
	    {
	      tree_members[idx1]->has_parent=TRUE;
	      curr_tree_member->child[--depth]=idx1;
	    }
	}
      if(depth!=0)
	exit_error("bug in  print_sum_infix\n");
    }
#ifdef HAVE_CONSTANTS_FILE
  for(pass=0;pass<(fundamental_list ? 2:1);pass++)
#endif
    {
#if MAX_NUM_LOOPVARS
      print_tabs(sum->num_loopvars);
      printf("result=");
#endif
      recurse_sum_infix(sum->stack_depth-1,sum);
      printf("\n");
    }
}
#endif

#ifdef HAVE_PRINT_SUM_RPN
void print_sum_rpn(sum_t *sum)
{
  int idx
#ifdef HAVE_CONSTANTS_FILE
    ,pass
#endif
    ;
  stack_entry *curr;
#ifdef HAVE_CONSTANTS_FILE
  for(pass=0;pass<(fundamental_list ? 2:1);pass++)
#endif
    {
#if MAX_NUM_LOOPVARS
      print_tabs(sum->num_loopvars);
      printf("result=");
#endif
      for(idx=0;idx<sum->stack_depth;idx++)
	{
	  curr=&sum->stack[idx];
#ifdef SIGNED_OPERATION
	  if(curr->minus)
	    printf("-");
#endif
	  switch(curr->tag)
	    {
#ifdef SEQUENCE_HUNTER
#ifdef HAVE_FUNCTIONS
	    case  function_tag:
	      printf("f ");
	      break;
#endif
	    case dimension_tag:
	      printf("n["DIMENSION_FORMAT"] ",curr->val);
	      break;
#endif
#ifdef HAVE_CONSTANTS_FILE
	    case constant_tag:
	      if(pass)
		printf(NUMBER_FORMAT" ",fundamental_list[curr->val]->value);
	      else
		printf("%s ",fundamental_list[curr->val]->name);
	      break;
#endif
#if MAX_NUM_LOOPVARS
	    case loopvar_tag:
	      printf("lv["LOOPVAR_FORMAT"] ",curr->val);
	      break; 
#endif
	    case integer_tag:
	      printf(STACKVAL_FORMAT" ",curr->val);
	      break;
	    case arithmetic_operation_tag:
	      printf(" %s ",operator_str[curr->val]);
	      break;
	    }
	}
      printf("\n");
    }
}
#endif

void print_sum_postamble(sum_t *sum)
{
#if MAX_NUM_LOOPVARS
  int idx;
  for(idx=sum->num_loopvars-1;idx>=0;idx--)
    {
      print_tabs(idx+1);
      printf("cum_result=cum_result %s result\n",operator_str[sum->loopvar_stack[idx].val]);
    }
#endif
  if(max_num_answers!=-1)
    {
      num_answers++;
      if(num_answers>=max_num_answers)
	exit(0);
    }
}

void print_sum(sum_t *sum)
{
  PRINT_SUM(sum);
}

#ifdef HAVE_ERROR_MEASUREMENTS
char *error_measurment_str[NUM_ERROR_MEASUREMENTS]=
  {
#ifdef HAVE_LMS_ERROR
    "lms",
#endif
#ifdef HAVE_ABS_ERROR
    "abs",
#endif
#ifdef HAVE_LMS_RATIOED_ERROR
    "lms ratioed",
#endif
#ifdef HAVE_ABS_RATIOED_ERROR
    "abs ratioed",
#endif
#ifdef HAVE_LMS_GT_ERROR
    "lms gt",
#endif
#ifdef HAVE_GT_ERROR
    "gt",
#endif
#ifdef HAVE_LMS_GT_RATIOED_ERROR
    "lms gt ratioed",
#endif
#ifdef HAVE_GT_RATIOED_ERROR
    "gt ratioed",
#endif
#ifdef HAVE_LMS_LT_ERROR
    "lms lt",
#endif
#ifdef HAVE_LT_ERROR
    "lt error",
#endif
#ifdef HAVE_LMS_LT_RATIOED_ERROR
    "lms lt ratioed",
#endif
#ifdef HAVE_LT_RATIOED_ERROR
    "lt ratioed",
#endif
  };

#ifdef HAVE_DEBUG_ERROR_LIST
void debug_error_list_func(calculate_sum_result *retval)
{
  result_t testvals=*get_array_member(array_indices);
#ifdef MULTIPLE_RESULTS
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
      printf("result="NUMBER_FORMAT" expected="NUMBER_FORMAT"\n",
	     result_stack[0],
#ifdef MULTIPLE_RESULTS
	     *answers++
#else
	     testvals
#endif
	     );
    }
}
#endif

void print_error_measurements()
{
  int idx;

  for(idx=0;idx<NUM_ERROR_MEASUREMENTS;idx++)
    {
      struct list_head *curr_error_list=&error_list[idx];
      error_list_element *element;
      if(!list_empty(curr_error_list))
	{
	  printf("%s error list\n",error_measurment_str[idx]);
	  list_for_each(((struct list_head *)element),curr_error_list)
	    {
	      printf("error="ERROR_FORMAT"\n",element->error_val);
	      print_sum(&element->sum);
#ifdef HAVE_DEBUG_ERROR_LIST
	      {
		calculate_sum_result result=calculate_sum(&element->sum,debug_error_list_func);
		if(result.aborted)
		  {
		    fprintf(stderr,"BUG DEBUG_ERROR_LIST sum is illegal");
		    print_sum(&element->sum);
		    exit(-1);
		  }
	      }
#endif
	    }
	}
    }
}
#endif

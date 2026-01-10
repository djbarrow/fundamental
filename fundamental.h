/*
This file is part of fundamental a brute force searcher
for relationships between constants & formulae for sequences.
Copyright (C) 2004  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

It is licensed under GPL v2.1
*/

#include "fundamental_config.h"
#if defined(SEQUENCE_HUNTER) || defined(REAL_HUNTER)
#define HUNTER
#endif

#if defined(HAVE_FACTORIAL_OP) || \
defined(HAVE_ABS_OP) || \
defined(HAVE_FLOOR_OP)|| \
defined(HAVE_CEIL_OP) || \
defined(HAVE_RINT_OP) || \
defined(HAVE_SIN_OP) || \
defined(HAVE_COS_OP) || \
defined(HAVE_TAN_OP) || \
defined(HAVE_ONES_COMPLIMENT_OP) || \
defined(HAVE_NOT_OP)
#define HAVE_UNARY_OPERATORS
#endif

#if defined(HAVE_ADDITION_OP) || \
defined(HAVE_SUBTRACTION_OP) || \
defined(HAVE_MULTIPLY_OP) || \
defined(HAVE_DIVIDE_OP) || \
defined(HAVE_POWER_OP) || \
defined(HAVE_MODULO_OP) || \
defined(HAVE_AND_OP) || \
defined(HAVE_OR_OP) || \
defined(HAVE_XOR_OP) || \
defined(HAVE_LOG_SHIFT_OP) || \
defined(HAVE_LOG_RSHIFT_OP) || \
defined(HAVE_LOG_LSHIFT_OP) || \
defined(HAVE_ARITH_SHIFT_OP) || \
defined(HAVE_ROTATE_OP) || \
defined(HAVE_MASK_OP) || \
defined(HAVE_GETBIT_OP) || \
defined(HAVE_LN_OP) || \
defined(HAVE_GCD_OP) || \
defined(HAVE_EQ_COND) || \
defined(HAVE_GT_COND) || \
defined(HAVE_GE_COND) || \
defined(HAVE_LT_COND) || \
defined(HAVE_LE_COND) || \
defined(HAVE_AND_COND) || \
defined(HAVE_OR_COND) || \
defined(HAVE_MIN_OP) || \
defined(HAVE_MAX_OP)
#define HAVE_BINARY_OPERATORS
#endif

#if defined(HAVE_FLOOR_OP)|| \
defined(HAVE_CEIL_OP) || \
defined(HAVE_RINT_OP) || \
defined(HAVE_SIN_OP) || \
defined(HAVE_COS_OP) || \
defined(HAVE_TAN_OP) || \
defined(HAVE_LN_OP)
#define HAVE_FLOATING_POINT_OPERATORS
#endif


#if defined(HAVE_AND_OP) || \
defined(HAVE_OR_OP) || \
defined(HAVE_XOR_OP) || \
defined(HAVE_LOG_SHIFT_OP) || \
defined(HAVE_LOG_RSHIFT_OP) || \
defined(HAVE_LOG_LSHIFT_OP) || \
defined(HAVE_ARITH_SHIFT_OP) || \
defined(HAVE_ROTATE_OP) || \
defined(HAVE_LROTATE_OP) || \
defined(HAVE_RROTATE_OP) || \
defined(HAVE_MASK_OP) || \
defined(HAVE_GETBIT_OP)
#define HAVE_BOOLEAN_OPERATORS
#endif

#if defined(HAVE_EQ_COND) || \
defined(HAVE_GT_COND) || \
defined(HAVE_GE_COND) || \
defined(HAVE_LT_COND) || \
defined(HAVE_LE_COND) || \
defined(HAVE_AND_COND) || \
defined(HAVE_OR_COND)
#define HAVE_CONDITIONALS
#endif

#if defined(HAVE_MIN_OP) || \
defined(HAVE_MAX_OP)
#define HAVE_MIN_MAX_OPERATORS
#endif

#if defined(HAVE_LOG_RSHIFT_OP) || \
defined(HAVE_LOG_LSHIFT_OP) || \
defined(HAVE_SUBTRACTION_OP) || \
defined(HAVE_LROTATE_OP) || \
defined(HAVE_RROTATE_OP)
#define HAVE_EXCESS_INTEGER_UNSIGNED_OPERATORS
#endif

#if defined(HAVE_LMS_ERROR) || \
defined(HAVE_ABS_ERROR) || \
defined(HAVE_LMS_RATIOED_ERROR) || \
defined(HAVE_ABS_RATIOED_ERROR) || \
defined(HAVE_LMS_GT_ERROR) || \
defined(HAVE_GT_ERROR) || \
defined(HAVE_LMS_GT_RATIOED_ERROR) || \
defined(HAVE_GT_RATIOED_ERROR) || \
defined(HAVE_LMS_LT_ERROR) || \
defined(HAVE_LT_ERROR) || \
defined(HAVE_LMS_LT_RATIOED_ERROR) || \
defined(HAVE_LT_RATIOED_ERROR) 
#define HAVE_ERROR_MEASUREMENTS
#endif


#if defined(HAVE_LMS_ERROR) || \
defined(HAVE_LMS_RATIOED_ERROR) || \
defined(HAVE_LMS_GT_ERROR) || \
defined(HAVE_LMS_GT_RATIOED_ERROR) || \
defined(HAVE_LMS_LT_ERROR) || \
defined(HAVE_LMS_LT_RATIOED_ERROR)
#define HAVE_LMS_ERROR_MEASUREMENTS
#endif

#if defined(HAVE_LMS_RATIOED_ERROR) || \
defined(HAVE_LMS_GT_RATIOED_ERROR) || \
defined(HAVE_LMS_LT_RATIOED_ERROR)
#define HAVE_LMS_RATIOED_ERROR_MEASUREMENTS
#endif


#if defined(HAVE_ABS_RATIOED_ERROR) || \
defined(HAVE_GT_RATIOED_ERROR) || \
defined(HAVE_LT_RATIOED_ERROR) 
#define HAVE_POWER1_RATIOED_ERROR_MEASUREMENTS
#endif

#if defined(HAVE_POWER1_RATIOED_ERROR_MEASUREMENTS) || \
defined(HAVE_LMS_RATIOED_ERROR_MEASUREMENTS)
#define HAVE_RATIOED_ERROR_MEASUREMENTS
#endif

#if defined(HAVE_LMS_GT_ERROR) || \
defined(HAVE_GT_ERROR) || \
defined(HAVE_LMS_GT_RATIOED_ERROR) || \
defined(HAVE_GT_RATIOED_ERROR)
#define HAVE_GT_ERROR_MEASUREMENTS
#endif

#if defined(HAVE_LMS_LT_ERROR) || \
defined(HAVE_LT_ERROR) || \
defined(HAVE_LMS_LT_RATIOED_ERROR) || \
defined(HAVE_LT_RATIOED_ERROR) 
#define HAVE_LT_ERROR_MEASUREMENTS
#endif





#include "fundamental_sanity.h"
#include "defs.h"

#ifndef NUM_INTEGER_BITS
#include <math.h>
#endif
//#define NDEBUG
#include <assert.h>

#ifdef NUM_INTEGER_BITS
#if NUM_INTEGER_BITS>32
#ifdef SIGNED_OPERATION
typedef long long number_t;
typedef long long uint_t;
#define NUMBER_FORMAT "%lld"
#else
typedef unsigned long long number_t;
typedef unsigned long long uint_t;
#define NUMBER_FORMAT "%llu"
#endif
/* atoll will break for unsigned numbers > signed long long */
#define ASCII_TO_NUM atoll
typedef unsigned long long unsigned_number_t;

#if NUM_INTEGER_BITS==64
#define RESULT_MASK (0xFFFFFFFFFFFFFFFFULL)
#else
#define RESULT_MASK (((1ULL<<NUM_INTEGER_BITS)-1))
#endif
#ifdef SIGNED_OPERATION
#define IS_NEGATIVE(val)  (val&(1ULL<<(NUM_INTEGER_BITS-1)))
#define MAKE_POSITIVE(val) (((val^RESULT_MASK)+1ULL)&RESULT_MASK)
#endif
#else /* NUM_INTEGER_BITS>32 */
#ifdef SIGNED_OPERATION
typedef long number_t;
typedef unsigned long uint_t;
#define NUMBER_FORMAT "%ld"
#define ASCII_TO_NUM atol
#else
typedef unsigned long number_t;
typedef unsigned long uint_t;
#define NUMBER_FORMAT "%lu"
#define ASCII_TO_NUM (number_t)atoll
#endif

typedef unsigned long unsigned_number_t;

#if NUM_INTEGER_BITS!=32
#define RESULT_MASK (((1UL<<NUM_INTEGER_BITS)-1))
#ifdef SIGNED_OPERATION
#define IS_NEGATIVE(val)  (val&(1UL<<(NUM_INTEGER_BITS-1)))
#define MAKE_POSITIVE(val) (((val^RESULT_MASK)+1UL)&RESULT_MASK)
#endif
#endif
#if defined (RESULT_MASK) && defined(SIGNED_OPERATION)
#define SIGN_EXTEND(val) (IS_NEGATIVE(val) ? ((val)|(~RESULT_MASK)):(val))
#else
#define SIGN_EXTEND(val)   (val)
#ifdef SIGNED_OPERATION
#define IS_NEGATIVE(val)  (val<0)
#define MAKE_POSITIVE(val) (-val)
#else
#define IS_NEGATIVE(val)  (0)
#define MAKE_POSITIVE(val) (val)
#endif
#endif
#endif
#else /* NUM_INTEGER_BITS */
#define SIGN_EXTEND(val)   (val)
#define IS_NEGATIVE(val)  (val<0)
#define MAKE_POSITIVE(val) (-val)
typedef long long int_t;
typedef unsigned long long uint_t;
#define MAX_INT_T  ((int_t)(~0ULL>>1))
#define MIN_INT_T  (int_t)((uint_t)(~0ULL>>1)+1)
#endif
#ifndef NUM_INTEGER_BITS
typedef double number_t;
#define NUMBER_FORMAT "%e"
#define ASCII_TO_NUM atof
#endif


#ifdef MULTIPLE_RESULTS
typedef struct
{
#ifdef NUM_ANSWERS
      number_t answer[NUM_ANSWERS];
#else
      int num_answers;
      number_t answer[0];
#endif
} 
#if defined(SPARSE_ARRAY_INDICES) || !defined(NUM_ANSWERS) 
result_array_t,*result_t;
#else
result_t;
#endif /* SPARSE_ARRAY_INDICES */
#else
typedef number_t result_t;
#endif /* MULTIPLE_RESULTS */


#ifdef MULTIPLE_RESULTS
#ifdef NUM_ANSWERS
#ifdef SPARSE_ARRAY_INDICES
extern result_t alloc_result(void);
#endif
#else /* NUM_ANSWERS */
extern result_t alloc_result(int num_answers);
#endif /* NUM_ANSWERS */
#endif

#ifdef REAL_HUNTER
#define MULTI_DIMENSIONAL (!defined(NUM_HUNTER_DIMENSIONS) || NUM_HUNTER_DIMENSIONS>1)
#ifndef NUM_HUMTER_DIMENSIONS
extern int NUM_HUMTER_DIMENSIONS;
#else
#ifdef SEQUENCE_HUNTER
#define MULTI_DIMENSIONAL (!defined(NUM_SEQUENCE_DIMENSIONS) || NUM_SEQUENCE_DIMENSIONS>1)
#endif
#endif
#endif


#ifdef SEQUENCE_HUNTER
typedef unsigned long dimension_t;

#ifdef HAVE_FUNCTIONS
extern dimension_t curr_seed;
#endif
#define MAX_DIMENSION ((dimension_t)-1)
#define DIMENSION_FORMAT "%lu"
extern int sequence_func(result_t *retnums,dimension_t *array_indices);
extern result_t *get_array_member(dimension_t *array_indices,dimension_t *idxptr);
#endif
#ifdef REAL_HUNTER
#define DIMENSION_FORMAT "%lu"
#endif


typedef int depth_t;
#define DEPTH_CHANGE_FORMAT "%d"

#ifdef HAVE_PRINT_SUM_INFIX
struct infix_tree;
struct infix_tree
{
      int has_parent;
      depth_t depth;
      depth_t child[];
};
extern struct infix_tree  **tree_members;
#endif

typedef enum
{
   null_op=-1,
   first_op=0,
#ifdef HAVE_UNARY_OPERATORS
   first_unary_op=first_op,
   dummy_op0=first_unary_op-1,
#ifdef HAVE_FACTORIAL_OP
   factorial_op,
#endif
#ifdef HAVE_ABS_OP
   abs_op,
#endif
#ifdef HAVE_FLOOR_OP
   floor_op,
#endif
#ifdef HAVE_CEIL_OP
   ceil_op,
#endif
#ifdef HAVE_RINT_OP
   rint_op,
#endif
#ifdef  HAVE_SIN_OP
   sin_op,
#endif
#ifdef HAVE_COS_OP
   cos_op,
#endif
#ifdef HAVE_TAN_OP
   tan_op,
#endif
#ifdef HAVE_ONES_COMPLIMENT_OP
   ones_compliment_op,
#endif
#ifdef HAVE_NOT_OP
   not_op,
#endif
   unary_op_idx,
   last_unary_op=unary_op_idx-1,
#endif /* UNARY_OPERATORS */
#ifdef HAVE_BINARY_OPERATORS
   first_binary_op
#ifndef HAVE_UNARY_OPERATORS
   =first_op
#endif
   ,
   dummy_op1=first_binary_op-1,
#endif
#ifdef HAVE_ADDITION_OP
   addition_op,
#endif
 #ifdef HAVE_SUBTRACTION_OP
   subtraction_op,
#endif
#ifdef HAVE_MULTIPLY_OP
   multiply_op,
#endif
#ifdef HAVE_DIVIDE_OP
   divide_op,
#endif
#ifdef HAVE_POWER_OP
   power_op,
#endif
#ifdef HAVE_MODULO_OP
   modulo_op,
#endif
#ifdef HAVE_AND_OP
   and_op,
#endif
#ifdef HAVE_OR_OP
   or_op,
#endif
#ifdef HAVE_XOR_OP
   xor_op,
#endif
#ifdef HAVE_LOG_SHIFT_OP
   log_shift_op,
#endif
#ifdef HAVE_LOG_RSHIFT_OP
   log_rshift_op,
#endif
#ifdef HAVE_LOG_LSHIFT_OP
   log_lshift_op,
#endif
#ifdef HAVE_ARITH_SHIFT_OP
   arith_shift_op,
#endif
#ifdef HAVE_ROTATE_OP
   rotate_op,
#endif
#ifdef HAVE_LROTATE_OP
   lrotate_op,
#endif
#ifdef HAVE_RROTATE_OP
   rrotate_op,
#endif
#ifdef  HAVE_MASK_OP
   mask_op,
#endif
#ifdef HAVE_GETBIT_OP
   getbit_op,
#endif
#ifdef HAVE_LN_OP
   ln_op,
#endif
#ifdef HAVE_GCD_OP
   gcd_op,
#endif
#ifdef HAVE_EQ_COND
   eq_cond,
#endif
#ifdef HAVE_GT_COND
   gt_cond,
#endif
#ifdef HAVE_GE_COND
   ge_cond,
#endif
#ifdef HAVE_LT_COND
   lt_cond,
#endif
#ifdef HAVE_LE_COND
   le_cond,
#endif
#ifdef HAVE_AND_COND
   and_cond,
#endif
#ifdef HAVE_OR_COND
   or_cond,
#endif
#ifdef HAVE_MIN_OP
   min_op,
#endif
#ifdef HAVE_MAX_OP
   max_op,
#endif
   binary_op_idx,
   last_binary_op=binary_op_idx-1,
   num_operators,
} operation;

extern depth_t op_depth[num_operators];

#ifdef HAVE_UNARY_OPERATORS
#define min_operator_depth (1)
#ifndef HAVE_BINARY_OPERATORS
#define max_operator_depth (1)
#endif
#else
#ifdef HAVE_BINARY_OPERATORS
#define min_operator_depth (2)
#endif
#endif

#ifdef HAVE_BINARY_OPERATORS
#define max_operator_depth (2)
#endif

#ifdef HAVE_CONSTANTS_FILE
struct fundamental_constant
{
      struct fundamental_constant *next;
      char             type;
      int              used;
      number_t           value;
      char             name[0];
};
typedef struct fundamental_constant fundamental_constant;
extern fundamental_constant *head,**fundamental_list;
extern int num_constants;
#endif


typedef enum
{
   invalid_tag=-1,
   first_tag,
   dummy_tag=first_tag-1,
#ifdef HUNTER
   dimension_tag,
#endif
#ifdef HAVE_CONSTANTS_FILE
   constant_tag,
#endif
   integer_tag,
   last_number_tag=integer_tag,
   arithmetic_operation_tag,
#ifdef HAVE_FUNCTIONS
   function_tag,
#endif
#ifdef HAVE_LOOPVAR
   loopvar_tag,
#endif
   dummy_tag0,
   max_tag=dummy_tag0-1
} stack_tag;


#ifdef  HAVE_SIMPLIFICATION_TEST
typedef unsigned simplify_result_tag_t;
void init_simplifyable(void);
#endif /* HAVE_SIMPLIFICATION_TEST */


#define is_operator(tag) ((tag)>last_number_tag) 
#define is_number(tag) ((tag)<=last_number_tag)
typedef long stackval_t;
#define STACKVAL_FORMAT "%ld"





#define abort_sum(err,format...) \
{ \
    debug_printf(format); \
    return(err); \
}


#ifdef MULTIPLE_RESULTS
dimension_t *retvals;
#ifdef NUM_ANSWERS
#define MAX_NUM_RESULTS (NUM_ANSWERS)
#else
dimension_t MAX_NUM_RESULTS;
#endif
#endif


extern int num_answers,max_num_answers;

#ifdef HAVE_ERROR_MEASUREMENTS
/* Better to leave as long double 12 byte float on intel to minimise risk of overflow */ 
typedef long double error_t;
#define ERROR_FORMAT "%Le"

enum
{
#ifdef HAVE_LMS_ERROR
	lms_error,
#endif
#ifdef HAVE_ABS_ERROR
	abs_error,
#endif
#ifdef HAVE_LMS_RATIOED_ERROR
	lms_ratioed_error,
#endif
#ifdef HAVE_ABS_RATIOED_ERROR
	abs_ratioed_error,
#endif
#ifdef HAVE_LMS_GT_ERROR
	lms_gt_error,
#endif
#ifdef HAVE_GT_ERROR
	gt_error,
#endif
#ifdef HAVE_LMS_GT_RATIOED_ERROR
	lms_gt_ratioed_error,
#endif
#ifdef HAVE_GT_RATIOED_ERROR
	gt_ratioed_error,
#endif
#ifdef HAVE_LMS_LT_ERROR
	lms_lt_error,
#endif
#ifdef HAVE_LT_ERROR
	lt_error,
#endif
#ifdef HAVE_LMS_LT_RATIOED_ERROR
	lms_lt_ratioed_error,
#endif
#ifdef HAVE_LT_RATIOED_ERROR
	lt_ratioed_error,
#endif
	NUM_ERROR_MEASUREMENTS
};
#endif

typedef struct
{
       stack_tag tag;
#ifdef SIGNED_OPERATION
       int minus;
#endif
      stackval_t val;
} stack_entry;

depth_t get_op_depth(stack_entry *curr);
typedef struct 
{
      int     stack_depth;
#ifdef HAVE_FUNCTIONS
      dimension_t seed;
#endif
      number_t *result_stack;
      stack_entry stack[];
} sum_t;


#if defined(MULTIPLE_RESULTS) || defined(HAVE_FUNCTIONS) 
extern void print_sum_preamble(sum_t *sum);
#define PRINT_SUM_PREAMBLE(sum) print_sum_preamble(sum);
#else
#define PRINT_SUM_PREAMBLE(sum)
#endif

#define PRINT_SUM_POSTAMBLE(sum) print_sum_postamble(sum);
#ifdef HAVE_PRINT_SUM_INFIX
extern void print_sum_infix(sum_t *sum);
#endif
#ifdef HAVE_PRINT_SUM_RPN
extern void print_sum_rpn(sum_t *sum);
#endif
extern void print_sum(sum_t *sum);

#if defined(HAVE_PRINT_SUM_INFIX) && defined(HAVE_PRINT_SUM_RPN)
#define PRINT_SUM(sum1) {print_sum_preamble(sum1) print_sum_rpn(sum1);print_sum_infix(sum1);print_sum_postamble(sum1)}
#endif
#if defined(HAVE_PRINT_SUM_RPN) && !defined(HAVE_PRINT_SUM_INFIX)
#define PRINT_SUM(sum1)\
   PRINT_SUM_PREAMBLE(sum1)\
   print_sum_rpn(sum1);\
   PRINT_SUM_POSTAMBLE(sum1)
#endif
#if defined(HAVE_PRINT_SUM_INFIX) && !defined(HAVE_PRINT_SUM_RPN) 
#define PRINT_SUM(sum1)\
   PRINT_SUM_PREAMBLE(sum1)\
   print_sum_infix(sum1);\
   PRINT_SUM_POSTAMBLE(sum1)
#endif /* defined(HAVE_PRINT_SUM_INFIX) && defined(HAVE_PRINT_SUM_RPN) */
#ifdef HAVE_ERROR_MEASUREMENTS
extern void print_error_measurements(void);
#endif



#ifdef HAVE_ERROR_MEASUREMENTS
#include "list.h"
typedef struct
{
      struct list_head list;
      error_t error_val;
      int     processed;
      sum_t   sum;
} error_list_element;

extern struct list_head error_list[NUM_ERROR_MEASUREMENTS];
#endif
typedef struct
{
#ifdef HUNTER
      int num_sequence_correct_count;
#endif
#if !defined(NUM_INTEGER_BITS) && !defined(ERROR_OP)
      number_t sum_correct_error_tolerance;
#endif
      int sum_correct;
      int aborted;
} calculate_sum_result;

typedef void (*calculate_sum_func_t)(calculate_sum_result *retval);
extern calculate_sum_result calculate_sum(sum_t *sum,calculate_sum_func_t sum_func);
#ifndef NUM_INTEGER_BITS
extern int number_to_int_t(number_t *numval,int_t *intval);
#endif
#ifdef SEQUENCE_HUNTER
//extern dimension_t *array_indices;
#endif
extern number_t *result_stack;
extern int max_stack_depth;
#ifdef REAL_HUNTER
extern number_t *coord_and_result_array;
extern int num_real_coord_idx;
#ifndef NUM_HUNTER_DIMENSIONS
extern number_t NUM_HUNTER_DIMENSIONS;
#endif
#endif
#ifdef SEQUENCE_HUNTER
#ifndef NUM_SEQUENCE_DIMENSIONS
extern dimension_t NUM_SEQUENCE_DIMENSIONS;
#endif 
#endif


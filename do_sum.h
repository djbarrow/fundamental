/*
This file is part of fundamental a brute force searcher
for relationships between constants & formulae for sequences.
It is a header for the reverse polish notation RPN calculation 
code in the project.
Copyright (C) 2018  D.J. Barrow dj_barrow@ariasoft.ie barrow_dj@yahoo.com

It is licensed under GPL v2.1
*/
#ifdef HAVE_GCD_OP
number_t euclid_calc_gcd(number_t x,number_t y);
#endif
#ifdef HAVE_FACTORIAL_OP
void init_factorials();
#endif
#ifdef HAVE_BINARY_OPERATORS
int do_binary_sum(number_t *result_ptr,number_t *operand,stack_entry *curr);
#endif
int do_sum(number_t **result_stack_head_ptrptr,stack_entry *curr);

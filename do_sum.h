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

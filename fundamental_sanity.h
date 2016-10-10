#ifdef NUM_INTEGER_BITS
#if (NUM_INTEGER_BITS>64||NUM_INTEGER_BITS<=0)
#error "Illegal NUM_INTEGER_BITS definition."
#endif
#ifdef HAVE_FLOATING_POINT_OPERATORS
#error "Floating point operators defined in integer mode."
#endif
#if defined(SIGNED_OPERATION) && defined (HAVE_EXCESS_INTEGER_UNSIGNED_OPERATORS)
#error "SIGNED_OPERATION & HAVE_EXCESS_INTEGER_UNSIGNED_OPERATORS defined"
#endif 
#else /* NUM_INTEGER_BITS */
#ifdef HAVE_BOOLEAN_OPERATORS
#error "Boolean operators defined in floating point mode." 
#endif
#endif /* NUM_INTEGER_BITS */

#if !defined(HAVE_PRINT_SUM_INFIX)&&!defined(HAVE_PRINT_SUM_RPN)
#error "HAVE_PRINT_SUM_INFIX or HAVE_PRINT_SUM_RPN should be defined"
#endif


#if defined(MULTIPLE_RESULTS)
#ifndef SEQUENCE_HUNTER
#error "MULTIPLE_RESULTS only compatible with SEQUENCE_HUNTER mode."
#endif
#else
#ifdef PRINT_MULTIPLE_ANSWERS
#error "PRINT_MULTIPLE_ANSWERS defined without MULTIPLE_RESULTS"
#endif
#ifdef NUM_ANSWERS
#error "NUM_ANSWERS defined without MULTIPLE_RESULTS"
#endif
#endif
#if !defined(SEQUENCE_HUNTER)&& !defined(HAVE_CONSTANTS_FILE)
#error "SEQUENCE_HUNTER or HAVE_CONSTANTS_FILE need to be defined"
#endif


#if defined(HAVE_BLOCK_DIST_RATIOED_ERROR) && defined(HAVE_DIST_RATIOED_ERROR)
#error "HAVE_BLOCK_DIST_RATIOED_ERROR && HAVE_DIST_RATIOED_ERROR both defined"
#endif

#if defined(HAVE_ERROR_MEASUREMENTS) && ! defined(SEQUENCE_HUNTER)
#error "SEQUENCE_HUNTER must be defined if HAVE_ERROR_MEASUREMENTS is defined"
#endif

#if defined(MULTIPLE_RESULTS) && defined(HAVE_ERROR_MEASUREMENTS)
#error "MULTIPLE_RESULTS & HAVE_ERROR_MEASUREMENTS are mutually exclusive & incompatible."
#endif

#if !defined(SIGNED_OPERATION) && defined(NUM_INTEGER_BITS) && defined(HAVE_ERROR_MEASUREMENTS)
#warning "un SIGNED_OPERATION && HAVE_ERROR_MEASUREMENTS normally doesn't work owing to casting problems"
#endif

#if  defined(HAVE_ERROR_MEASUREMENTS) && defined(MAX_NUM_LOOPVARS)
#error "These are not yet designed to work together see bottom of increment_sum_order."
#endif
#if defined(HAVE_DEBUG_ERROR_LIST) && !defined (HAVE_ERROR_MEASUREMENTS)
#error "HAVE_DEBUG_ERROR_LIST definition needs HAVE_ERROR_MEASUREMENTS definition."
#endif

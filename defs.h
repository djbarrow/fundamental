#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<=(b) ? (a):(b))
#endif
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef NULL
#define NULL ((void *)0)
#endif

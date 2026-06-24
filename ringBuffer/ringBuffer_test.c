
#define CIRCBUF_DEF(x,y) uint32_t x##_space[y]; circBuf_t x = { x##_space,0,0,y}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <assert.h>
#include <string.h> //memcpy
#include "ringBuffer.h"


#define START_NEW_TEST(test_num) \
    fprintf(stderr, "Test %d...", (++test_num));

#define END_TEST(test_num) \
    fprintf(stderr, "pass.\n");

/* Default size for tests. */
#define kNumPointsInMyBuffer 100  //maxLen
#define initialSize 200		// size

int main()
{
	/*
	When
*/
	// initilize ring Buffer	
	const int maxLen = kNumPointsInMyBuffer;

	// src:
	int src[1024] = {};
	int i;
	for(i=0; i<1024; i++){
		src[i] = i+1;
	}

	//data
	uint32_t data[1024];	
	memcpy(data, src, 1024);

/*	printf("\nThe size of the uint32_t data array is %lu\n", sizeof(data));*/
	int sz = sizeof(*data);

	circBuf_t *cb;
	cb = malloc(sizeof(circBuf_t));
	circBuf_init(cb, maxLen, sz);  //initilization
	uint32_t *x;
	uint32_t a;

	int test_num = 0;


/*printf("sizeof(uint32_t) = %ld\n", sizeof(uint32_t));*/
/*printf("sizeof(int) = %ld\n", sizeof(int));*/


    /* Initialize buffer */
    START_NEW_TEST(test_num);
    assert(circBufCurrentSize(cb) == 0);
	x = circBufPeek(cb);
/*	printf("circBufPeek is %4u\n", *x);*/
	assert(!(*x) );						
    assert(circBufMaximumSize(cb) == maxLen-1);
/*    assert(circBufMaximumSize(cb) == 15);*/
    assert(circBufEmpty(cb) == 1);
    assert(circBufFull(cb) == 0);
    END_TEST(test_num);


	
/*	test circBufPush(circBuf_t *c, uint32_t *data, int num), the number of pushed data is smaller than capacity*/
/*	test circBufPeek(circBuf_t *c )*/
	circBufPush(cb, data, initialSize); 

    START_NEW_TEST(test_num);
/*printf("circBufCurrentSize(cb) is %d\n", circBufCurrentSize(cb));*/
    assert(circBufCurrentSize(cb) == initialSize);
	x = circBufPeek(cb);
/*	printf("circBufPeek is %4u\n", *x);*/
	assert((*x) );
    assert(circBufMaximumSize(cb) == maxLen-1);
    assert(circBufEmpty(cb) == 0);
    assert(circBufFull(cb) == 0);
    END_TEST(test_num);



/*test circBufPop(circBuf_t *c, int num) and circBufPop_one(circBuf_t *c)*/
    START_NEW_TEST(test_num);
/*printf("circBufCurrentSize(cb) is %d\n", circBufCurrentSize(cb));*/
	circBufPop_one(cb);
	circBufPop_one(cb);
	circBufPop(cb, 3);
    assert(circBufCurrentSize(cb) == initialSize-5);
	circBufPop(cb, 5);
	assert(circBufCurrentSize(cb));
    assert(circBufEmpty(cb) == 0);
	circBufPop_one(cb);
/*printf("circBufCurrentSize(cb) is %d\n", circBufCurrentSize(cb));*/
	circBufPop(cb, 20);
    assert(circBufEmpty(cb) == 1);
    END_TEST(test_num);
/*	printf("cb->tail= %d\n", cb->tail);*/




/*	test circBufPush, the number of pushed data is larger than capacity*/
	circBufPush(cb, data, initialSize*2);
    START_NEW_TEST(test_num);
	assert(circBufFull(cb) == 0);
	circBufPop(cb, kNumPointsInMyBuffer);
	assert(circBufEmpty(cb) == 1);
	circBufPush(cb, data, initialSize*2);
	circBufPop_one(cb);
	assert(circBufFull(cb) == 0);
/*printf("circBufCurrentSize(cb) is %d\n", circBufCurrentSize(cb));*/
    assert(circBufCurrentSize(cb) == initialSize*2-1);
	circBufPop(cb, 3);
	circBufPop(cb, 5);
	assert(circBufCurrentSize(cb));
    END_TEST(test_num);




/*free the memory*/
    START_NEW_TEST(test_num);
	assert(*cb);
/*	printf("cb's value is %p\n", cb);*/
	circBuf_free(cb);
/*	printf("cb's value is %p\n", cb);*/
	assert(!*cb);
    END_TEST(test_num);

return 1;

}

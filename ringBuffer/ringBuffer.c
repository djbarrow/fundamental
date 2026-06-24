/*
 * ringBufher.c - C ring buffer (FIFO) implementation.
 *
 * Written in 2016 by Di Li <li@umic.rwth-aachen.de>.
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to
 * the public domain worldwide. This software is distributed without
 * any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */


/*#undef NDEBUG*/
/*#include "ringBuffer.h"*/
#include <stdint.h>
//#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memcpy



typedef struct CircBuf_t //struct name CircBuf_t
{
    uint32_t *buffer;
    int head; // keep track the newest data
    int tail; // keep track the oldest data
    int maxLen; // maximum number of items in the buffer
}circBuf_t; //type name circBuf_t


	
// initialize the circular buffer
void circBuf_init(circBuf_t *c, const int maxLen, int sz)
{
	//printf("circBuf_init: maxLen= %d, sz =%d\n", maxLen, sz);

	c->buffer = malloc(maxLen * sz);
	c->maxLen = maxLen;
    if(c->buffer == NULL)
        printf("Buffer initialization fails\n");
    c->head = 0;
    c->tail = 0;
}



/* free the memory, free c->buffer first, then c*/
void circBuf_free(circBuf_t *c)
{
/*	printf("circBuf_free...\n");*/
    free(c->buffer);
	free(c);
}


//Push data of type uint32_t
int circBufPush(circBuf_t *c, uint32_t *data, int num)
{
/*	printf("\ncircBufPush...\n");  */
    int next = c->head + 1;
    if (next >= c->maxLen) // data can wrap back around to the beginning again to use up the extra space.
        next = 0;

    if (next == c->tail){
	 	printf("buffer is full!\n");
        return -1;  // quit with an error
		}


	int idx, i;
	uint32_t *temp; // as a base position
/*printf("num = %d\n", num );  */
	for (i =0; i< num; i++){
/*		idx = (i+ c->head  + 1) & (c->maxLen - 1);*/
		if(i+ c->head +1 > c->maxLen -1){
			idx = 	i+ c->head+1 -(c->maxLen - 1);
		}else{
			idx = i+ c->head+1;
		}
/*		printf("idx = %d\n", idx); */
/*	printf("c->maxLen = %d\n", c->maxLen);  */
		if(idx  == c->tail){
		 	printf("buffer is full!!\n");  
			c->head = idx -1;
		    return 0;  // quit with an error
		}
		idx--;
		temp = c->buffer;
		temp += idx;
		*temp = *(data+i);
/*		printf("circBufPush, *(temp)[%d] is %4u\n", idx, *temp);*/
		}

// Update the current index of our ring buffer.
	c->head += num;
/*	printf("c->head is %d\n", c->head);*/
/*    c->head &= c->maxLen -1;*/
	if(c->head > c->maxLen -1){
		c->head = c->head -(c->maxLen - 1);
	}
/*	printf("c->head is %d\n", c->head);*/
    return 1;
}
 


//Pop a chunck of values
int circBufPop(circBuf_t *c, int num)
{
/*	printf("\ncircBufPop...\n");*/

    if (c == NULL || c->buffer == NULL)
        return 0;

    if (c->head == c->tail)
        return -1;  // empty, quit with an error

    int currentSize = circBufCurrentSize(c);

	if(num >= currentSize){
		printf("The buffer is cleared!\n");
		c->head =0;
		c->tail =0;
	}
	else{
		int i, idx;
		uint32_t *temp;
		for(i=0; i<num; i++){
/*			idx = (i+ c->tail) & (c->maxLen -1);*/
			if(i+ c->tail > c->maxLen -1){
				idx = i+ c->tail - (c->maxLen -1);
				}else{
					idx = i+ c->tail;				
				}
			temp = c->buffer;
			temp += idx;
/*			printf("the value to be popped up is %4u\n", *temp);*/
			*temp = 0;
			}
		if(c->tail + num > c->maxLen -1){
			c->tail = c->tail + num - (c->maxLen -1);			
			}else{
			c->tail = c->tail + num;
}

/*		printf("c->tail is %4u\n", c->tail);*/
		}
    return 0;
}


//return the oldest value.
uint32_t circBufPop_one(circBuf_t *c)
{
/*	printf("\ncircBufPop...\n");*/
    if (c->head == c->tail)
        return -1;  // empty, quit with an error

	uint32_t *temp;
	uint32_t a;
	temp = c->buffer;
	temp += c->tail;


/*	printf("\n the value to be popped is %4u\n", *temp);*/
	c->tail = (c->tail + 1) & (c->maxLen -1);
/*	printf("\n c->tail=%d\n", c->tail);*/
	a= *temp;
	*temp = 0;
	return a;
}


// Current Size
int circBufCurrentSize(circBuf_t *c)
{
    // if the head isn't ahead of the tail, we don't have any characters
/*	printf("\ncircBufCurrentSize...\n");*/
    if (c->head == c->tail)
        return 0;  // empty
 
    if(c->head +1 == c->tail){
	printf("\nxxxxx c->head =%d, and c->tail= %d \n", c->head, c->tail);
		return c->maxLen -1; //full
	}
 
    if(c->tail < c->head){
		return c->head - c->tail;
	}
	if(c->head < c->tail){
		return	c->maxLen - c->tail + c->head;
	}
}


//Peek the oldest data
uint32_t *circBufPeek(circBuf_t *c ){
	uint32_t *data;
	data = c->buffer;
	int temp_tail = c->tail;
	while(temp_tail--){
		data++;		
	}

/*	printf("circBufPeek...%4u\n	", *data);*/
	return data;
	}


// Maximum Size
int circBufMaximumSize(circBuf_t *c){
		return c->maxLen -1;
	}


//check whether buffer is empty
int circBufEmpty(circBuf_t *c){
	if (c->head == c->tail)
        return 1;
	else 
		return 0;
	}

//check whether buffer is full
int circBufFull(circBuf_t *c){
	if (c->head +1  == c->tail)
		return 1;
	else 
		return 0;
	}



 

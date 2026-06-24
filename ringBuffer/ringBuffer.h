#ifndef INCLUDED_RINGBUF_H
#define INCLUDED_RINGBUF_H

/*
 * ringbuf.h - C ring buffer (FIFO) interface.
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

/*
 * A byte-addressable ring buffer FIFO implementation.
 *
 * The ring buffer's head pointer points to the starting location
 * where data should be written when copying data *into* the buffer
 * (e.g., with ringbuf_read). The ring buffer's tail pointer points to
 * the starting location where data should be read when copying data
 * *from* the buffer (e.g., with ringbuf_write).
 */



// user defined number of uint32_t types
// implement a circular buffer, the size of the buffer is power of two.



#include <stdint.h>
//#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memcpy



typedef struct circBuf_t *circBuf_t;


// initialize the circular buffer
void 
circBuf_init(circBuf_t *c, const int maxLen, int sz);


void 
circBuf_free(circBuf_t *c);

//Push
int 
circBufPush(circBuf_t *c, uint32_t *data, int num);
 

//Pop a chunck of values
int 
circBufPop(circBuf_t *c, int num);

//return the oldest value.
uint32_t
circBufPop_one(circBuf_t *c);

// Current Size
int 
circBufCurrentSize(circBuf_t *c);


//Peek the oldest data
uint32_t *
circBufPeek(circBuf_t *c );


// peek the first n elements.
int 
circBufPeek_n(circBuf_t *c, int n );


// Maximum Size
int 
circBufMaximumSize(circBuf_t *c);


//check whether buffer is empty
int 
circBufEmpty(circBuf_t *c);

//check whether buffer is full
int 
circBufFull(circBuf_t *c);


#endif /* INCLUDED_RINGBUF_H */



 

#ifndef RINGBUFFER_H
#define RINGBUFFER_H


typedef struct ringbuff ringbuff_t;

void ringbuff_alloc(ringbuff_t *r, const int num_members, int sizeof_member);
void ringbuff_free(ringbuff_t *r);
char *ringbuff_getbuff(ringbuff_t *r);
void ringbuff_enqueue(ringbuff_t *r);
void *ringbuff_dequeue(ringbuff_t *r);
void ringbuff_release(ringbuff_t *r);
int ringbuff_empty(ringbuff_t *r);
int ringbuff_full(ringbuff_t *r);
#endif

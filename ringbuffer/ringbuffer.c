#include "ringbuffer.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memcpy
#include "../utils.h"

/* Globals */
pthread_mutex_t queue_lock;

typedef struct ringbuff
{
  char *buffer;
  int head; 
  int tail; 
  int num_members; 
  int sizeof_member;
  sem_t enqueue;          // Wait for data to be removed to enqueue
  sem_t dequeue;          // Wait for data to be added to be dequeued
  pthread_mutex_t *lock;  // Mutex to lock data access to the struct
} ringbuff_t; 


	
void ringbuff_alloc(ringbuff_t *r, const int num_members, int sizeof_member)
{
  r->buffer = myalloc("r->buffer",num_members * sizeof_member);
  r->num_members = num_members;
  r->head = 0;
  r->tail = 0;
  pthread_mutex_init(&queue_lock, NULL);
  r->lock = &queue_lock;
  sem_init(&r->dequeue, 0, 0);
  // Initialize enqueue to the capacity of the queue, as we can place
  // Size amount of items in it from the get go.
  sem_init(&r->enqueue, 0, num_members);
}




void ringbuff_free(ringbuff_t *r)
{
  pthread_mutex_lock(r->lock);
  free(r->buffer);
  free(r);
  pthread_mutex_unlock(r->lock);

	// Free ALL THE THINGS!
  sem_destroy(&r->enqueue);
  sem_destroy(&r->dequeue);
  pthread_mutex_destroy(r->lock);
}
char *ringbuff_getbuff(ringbuff_t *r)
{
  sem_wait(&r->enqueue);
  pthread_mutex_lock(r->lock);
  char *retval=&r->buffer[r->head*r->sizeof_member];
  pthread_mutex_unlock(r->lock);
  return retval;
}
//Push data of type uint32_t
void ringbuff_enqueue(ringbuff_t *r)
{
  int next;
  // Update the last node with a pointer to the next item
  pthread_mutex_lock(r->lock);
     
  next = r->head + 1;
  if (next >= r->num_members) // data can wrap back around to the beginning again to use up the extra space.
    next = 0;

  if (next == r->tail){
    exit_error("buffer is full!\n");
  }
  r->head=next;
  pthread_mutex_lock(r->lock);
  // Issue update stating item to be dequeued
  sem_post(&r->dequeue);
}
 
//return the oldest value.
void *ringbuff_dequeue(ringbuff_t *r)  
{
  
  sem_wait(&r->dequeue);
  // Update the last node with a pointer to the next item
  pthread_mutex_lock(r->lock);
  int next = r->tail + 1;
  if (next >= r->num_members) 
    next = 0;
  char *retval=&r->buffer[r->tail*r->sizeof_member];
  r->tail=next;
  pthread_mutex_unlock(r->lock);
  return retval;
}
void ringbuff_release(ringbuff_t *r)
{
  sem_post(&r->enqueue);
}

//check whether buffer is empty
int ringbuff_empty(ringbuff_t *r)
{
  pthread_mutex_lock(r->lock);
  if (r->head == r->tail)
    {
    pthread_mutex_unlock(r->lock);
    return 1;
    }
  else
    {
      pthread_mutex_unlock(r->lock);
      return 0;
    }
}

//check whether buffer is full
int ringbuff_full(ringbuff_t *r)
{
  pthread_mutex_lock(r->lock);
  if (r->head +1  == r->tail)
    {
      pthread_mutex_unlock(r->lock);
      return 1;
    }
  else
    {
      pthread_mutex_unlock(r->lock);
      return 0;
    }
}

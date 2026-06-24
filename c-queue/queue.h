#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>


/*
 * Queue - the abstract type of a concurrent queue.
 */
typedef struct QueueStruct Queue;
typedef struct NodeStruct Node;
/**
 * Enables the creation and maintainance of a linked list.
 *
 * @param   item    A pointer to an item.
 * @param   next    The next Item in the list.
 */
/**
 * Enables the creation of a Thread-Safe Concurrent FIFO queue.
 *
 * @param   front           Current bytes to where the front of the
 *                          queue is.
 * @param   rear            Current bytes to where the rear of the queue
 *                          is.
 * @param   size            Total size in bytes of the fixed size queue.
 * @param   empty           Boolean integer to confirm whether queue is
 *                          empty.
 * @param   full            Boolean integer to confirm whether queue is
 *                          full.
 * @param   dequeue_lock    Semaphore lock for dequeuing.
 * @param   enqueue_lock    Semaphore lock for enqueuing.
 * @param   data            The actual data contained within the Queue.
 *
 */
typedef struct QueueStruct {
    int length;             // Current items in queue
    int capacity;           // Maximum operational size
    bool is_empty;          // True if queue empty
    bool is_full;           // True if queue full
    sem_t enqueue;          // Wait for data to be removed to enqueue
    sem_t dequeue;          // Wait for data to be added to be dequeued
    pthread_mutex_t *lock;  // Mutex to lock data access to the struct
    Node *root;             // First node in linked list
    Node *last;             // Last node in linked list
} Queue;


struct NodeStruct {
    void *item;
    Node *next;
    Node *last;
};


/*
 * queue_alloc:
 *
 * Allocate a concurrent queue of a specific size.
 *
 */
Queue *queue_alloc(int size);


/*
 * queue_free:
 *
 * Free a concurrent queue and associated memory.
 * NOTE: A user should not free a queue until any
 * users are finished. Any calling queue_free() while
 * any consumer/producer is waiting on queue_put or queue_get
 * will cause queue_free to print an error and exit the program.
 *
 */
void queue_free(Queue *queue);


/*
 * queue_put:
 *
 * Place an item into the concurrent queue.
 *
 * If there is no space available then queue_put will
 * block until a space becomes available when it will
 * put the item into the queue and immediately return.
 *
 * Uses void* to hold an arbitrary type of item,
 * it is the users responsibility to manage memory
 * and ensure it is correctly typed.
 *
 */
void enqueue(Queue *queue, void *item);


/*
 * queue_get:
 *
 * Get an item from the concurrent queue.
 *
 * If there is no item available then queue_get
 * will block until an item becomes avaible when
 * it will immediately return that item.
 *
 */
void *dequeue(Queue *queue);
int queue_length(Queue *queue);

#endif


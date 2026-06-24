/**
 * A concurrent FIFO Queue written in C. Takes in any type of data as
 * underlying implementation is a linked list containing pointers to
 * the actual data. Needs to be cast to (void *).
 *
 * Uses POSIX semaphores and mutexes. Tested under linux, unsure of
 * operation on Windows.
 *
 * Usage:
 *      Queue *queue_alloc(int size);
 *      void enqueue(Queue *queue, void *item);
 *      void *dequeue(Queue *queue);
 *
 * Example:
 *   This is just a struct for the purposes of providing an example.
 *      typedef struct {
 *          int value;
 *      } Task;
 *
 *   Initalisation of a queue
 *      Queue *queue = queue_alloc(QUEUE_SIZE);
 *
 *   Enqueuing (a Task) an item into a given Queue.
 *      Task *task = (Task*)malloc(sizeof(Task));
 *      task->value = i;
 *      enqueue(queue, task);
 *
 *   Dequeuing (a Task) an item from a given Queue.
 *      Task *task = (Task*)dequeue(queue);
 *
 * @author: Matt Hartstonge (2015)
 *
 */

/* Library includes */
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

/* Queue declarations, dependencies and globals */
/* Header Includes */
#include "queue.h"

/* Globals */
pthread_mutex_t queue_lock;






/**
 * Used to initilise a queue to a specified length.
 *
 * @param   size    Intial size in bytes to make the fixed size queue.
 * @return  queue   Struct of type QueueStruct.
 */
Queue *queue_alloc(int size) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    // Initialize Mutex lock
    pthread_mutex_init(&queue_lock, NULL);
    queue->lock = &queue_lock;

    queue->length = 0;
    queue->capacity = size;
    queue->is_empty = true;
    if (queue->length == queue->capacity){
        queue->is_full = true;
    } else {
        queue->is_full = false;
    }
    queue->root = (Node *)NULL;
    queue->last = (Node *)NULL;

    // Initialize dequeue to 0 as there are no items to dequeue yet
    sem_init(&queue->dequeue, 0, 0);
    // Initialize enqueue to the capacity of the queue, as we can place
    // Size amount of items in it from the get go.
    sem_init(&queue->enqueue, 0, size);
    return queue;
}

/**
 * Enqueues an item to a queue and creates the associated node record.
 *
 * If there is no space available, queue_put will block until a space
 * becomes available when it will put the item into the queue and
 * immediately return.
 *
 * Uses void* to hold an arbitrary type of item, it is the users
 * responsibility to manage memory and ensure it is correctly typed.
 * Thread Safe: Makes use of the Semaphore locks from QueueStruct.
 *
 * @param   queue   Struct of type Queue.
 * @param   item    Returns the linked item.
 */
void enqueue(Queue *queue, void *item){
    // Decrease count of eligible items to be enqueued
    sem_wait(&queue->enqueue);
        // Create a new node to add to queue
        Node *new_node = (Node *)malloc(1*sizeof(Node));
        new_node->item = (void *) item;
        new_node->next = NULL;
        new_node->last = NULL;

        // Update the last node with a pointer to the next item
        pthread_mutex_lock(queue->lock);
            if (queue->is_empty){
                // There is nothing in the queue
                queue->root = (Node *)new_node;
                queue->last = (Node *)new_node;
            } else {
                // Make the last entry point at the New Node
                queue->last->next = (Node *)new_node;
                new_node->last = (Node *)queue->last;
                // Make the New Node the new last entry!
                queue->last = (Node *)new_node;
            }

            // Update Queue Variables
            queue->length += 1;
            queue->is_empty = false;
            if (queue->length == queue->capacity){
                queue->is_full = true;
            }

            // Do last checks for overflows
            if (queue->length > queue->capacity){
	      char *error_message=alloca(1024);
                sprintf(error_message,
                        "Queue Length: %d\nQueue Capacity: %d\n",
                        queue->length,
                        queue->capacity
                );
                perror(error_message);
                perror("Buffer Overflow!");

            }
        pthread_mutex_unlock(queue->lock);
    // Issue update stating item to be dequeued
    sem_post(&queue->dequeue);
}

int queue_length(Queue *queue)
{
  return queue->length;
}
/**
 * Dequeues an item from a queue and frees the associated node record.
 *
 * If there is no item in the queue to get, it will block until item
 * is able to be got.
 *
 * Uses void* to hold an arbitrary type of item, it is the users
 * responsibility to manage memory after item is returned and ensure it
 * is correctly typed.
 *
 * Thread Safe: Makes use of the Semaphore locks from QueueStruct.
 *
 * @param   queue   Struct of type Queue.
 * @return  item    (void *) Returns the linked item.
 */
void *dequeue(Queue *queue){
    // Wait for an item to dequeue
    sem_wait(&queue->dequeue);
    if(queue->root==NULL)
      return NULL;
        Node *old_node = (Node *)NULL;      // The node to be destroyed
        Node *new_root = (Node *)NULL;      // The node before old node
        void *item = (void *)NULL;          // The item to return

        // Lock access to the data structure
        pthread_mutex_lock(queue->lock);
            if (queue->root == NULL){
                perror("Dude, you're dequeuing NULL... wtf? error, like, duh!");
            }

            // Get node to be destroyed and retrieve item
            old_node = (Node *)queue->root;
            item = (void *)old_node->item;

            // Update root node
            if (queue->root->next != NULL){
                // Update next pointers
                new_root = (Node *)queue->root->next;
                new_root->last = (Node *)NULL;
                queue->root = (Node *)new_root;
            } else {
                queue->root = NULL;
            }
            // Free the old node from memory
            free(old_node);

            // Adjust Queue variables
            queue->length -= 1;
            if (queue->length == 0){
                queue->root = (Node *)NULL;
                queue->last = (Node *)NULL;
                queue->is_empty = true;
            }
            if (queue->length < queue->capacity){
                queue->is_full = false;
            }
            if (queue->length < 0){
                perror("Error: Buffer underflow?");
            }
        pthread_mutex_unlock(queue->lock);
    // We can now enqueue another item
    sem_post(&queue->enqueue);
    return item;
}


/**
 * Frees a concurrent queue and associated memory.
 *
 * NOTE: A user should not free a queue until all users are finished.
 * Any calling queue_free() while any consumer/producer is waiting on
 * queue_put or queue_get will cause queue_free to print an error and
 * exit the program.
 */
void queue_free(Queue *queue) {
    // Check if data in use or hasn't been cleared properly.
    pthread_mutex_lock(queue->lock);
        if (queue->root != NULL || queue->last != NULL){
            perror("Data still held in the queue!! You 'dun goofed!");
            exit(1);
        }
    pthread_mutex_unlock(queue->lock);

    // Free ALL THE THINGS!
    sem_destroy(&queue->enqueue);
    sem_destroy(&queue->dequeue);
    pthread_mutex_destroy(queue->lock);
#if 0    
    if(queue->root)
      free(queue->root);
    if(queue->last&&queue->last!=queue->root)
      free(queue->last);
#endif    
    free(queue);
}


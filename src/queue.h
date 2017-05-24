
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include "internals.h"

typedef struct node IG_QElement;

struct node{
	IG_QElement * next;
	IG_Data * data;
};

typedef enum {IG_QUEUE_BLOCKING, IG_QUEUE_NONBLOCKING} IG_QueueType;

typedef struct{
	IG_UInt32 size;
	IG_QElement * front;
	IG_QElement * end;
	pthread_mutex_t mutex;
	sem_t size_sem;
	IG_QueueType type;
} IG_Queue;

//Creates a new empty Queue on the heap and returns a pointer to it
// +type: IG_QUEUE_BLOCKING or IG_QUEUE_NONBLOCKING
IG_Queue * IG_Queue_new(IG_QueueType type);

void IG_Queue_delete();

// Insert new data to the queue
void IG_Queue_put(IG_Queue * queue, IG_Data * new_Data);

void IG_Queue_put_copy(IG_Queue * queue, IG_Data new_Data);

// Return first element of the queue
//  returns nullptr on nonblockingqueue if the queue is Empty
//  blocks until there is an element on blockingqueue
IG_Data * IG_Queue_take(IG_Queue * queue);

//returns true if the queue is empty
IG_Bool IG_Queue_isEmpty(IG_Queue * queue);


#endif

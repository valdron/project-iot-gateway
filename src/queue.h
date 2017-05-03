
#include "internals.h"
#include "pthread.h"

typedef struct IG_Queue{
	IG_UInt32 size,
	IG_QElement * front,
	IG_QElement * end,
	pthread_mutex_t * mutex
} IG_Queue;

typedef struct IG_QElement{
	IG_QElement * next,
	IG_Data * data
} IG_QElement;

//Creates a new empty Queue on the heap and returns a pointer to it
IG_Queue * IG_Queue_new();

// Insert new data to the queue
void IG_Queue_put(IG_Queue * queue, IG_Data * new_Data);
void IG_Queue_put(IG_Queue * queue, IG_Data new_Data);

// Return first element of the queue
// or returns IG_DATA_EMPTY if the queue is Empty
IG_Data * IG_Queue_take(IG_Queue * queue);

//returns true if the queue is empty
IG_Bool IG_Queue_isEmpty(IG_Queue * queue);

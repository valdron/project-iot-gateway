
#include "internals.h"
//TODO: implement threadsafe queue




//structs !!MAY!! look like below
typedef struct IG_Queue{
    IG_UInt32 size,
    IG_QElement * front,
    IG_QElement * end,
} IG_Queue;

typedef struct IG_QElement{
    IG_Data 
} IG_QElement;

//Creates a new empty Queue on the heap and returns a pointer to it
IG_Queue * IG_Queue_new();

// enqueues the new data at the end of the queue
// should be thread-safe (mutex)
// Creates a new QElement and allocates the data on the heap
void IG_Queue_put(IG_Queue * queue, IG_Data data);

// takes the data form the front of the Queue
// or returns IG_DATA_EMPTY if the queue is Empty
IG_Data IG_Queue_take(IG_Queue * queue)

//returns true if the size is 0
IG_Bool IG_Queue_isEmpty();
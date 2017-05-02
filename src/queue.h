
#include "internals.h"
#include "pthread.h"	//TODO: ACHTUNG, pthread.h wird evtl. mehrmals includiert, mit Präprozesser
			//Anweisungen umgehen??? #ifndef ... / #define ... / include "..."

//TODO: implement threadsafe queue

//structs !!MAY!! look like below
typedef struct IG_Queue{
	IG_UInt32 size;
	IG_QElement * front;
	IG_QElement * end;
	pthread_mutex_t * mutex;
} IG_Queue;

typedef struct IG_QElement{
	IG_QElement * next;
	IG_Data * data;
} IG_QElement;

//Creates a new empty Queue on the heap and returns a pointer to it
IG_Queue * IG_Queue_new();

// enqueues the new data at the end of the queue
// should be thread-safe (mutex)
// Creates a new QElement and allocates the data on the heap

// TODO:Change: Second parameter is now a pointer
// Changed parameter IG_Data to IG_Data*
// Reason= IG_Data contains pointers, they get lost on Call-By-Value
void IG_Queue_put(IG_Queue * queue, IG_Data * new_Data);
// Incase if someone create it on thysax
void IG_Queue_put(IG_Queue * queue, IG_Data new_Data);

// takes the data form the front of the Queue
// or returns IG_DATA_EMPTY if the queue is Empty
// TODO: checking if its empty could be also done if the caller checks the queue->size himself
// This could avoid an unnecessary function call
IG_Data * IG_Queue_take(IG_Queue * queue);

//returns true if the size is 0
// TODO: Which queue? Need a pointer here to the queue, or we access directly the size variable
IG_Bool IG_Queue_isEmpty(IG_Queue * queue);

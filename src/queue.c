#include "queue.h"

IG_Queue * IGQueue_new(){

	// Init queue
	IG_Queue* queue = (IQ_Queue*)malloc(sizeof(IG_Queue));
	queue->size = 0;
	queue->front = NULL;
	queue->end = NULL;

	// Init mutex for this specific queue
	pthread_mutex_init(queue->mutex,NULL);
	return queue;
}

// Function to enqueue data
void IG_Queue_put(IG_Queue* queue, IG_Data* new_data){

	// Trying to lock this queue from other threads, if it's already locked this thread will
	// get locked and will be blocked until the mutex is unlocked by another thread
	pthread_mutex_lock(queue->mutex);

	// Creating new element for the queue
	IG_QElement* element = (IG_QElement*)malloc(sizeof(IG_QElement));
	
	// TODO: Sollen die Daten einfach der pointer weiter ggb. werden oder neu im HEAP?
	// Mit pointer weniger aufwand wegen kopieren der pointer and weniger speicherverbrauch 	
	element->data = new_data;
	element->next = NULL;

	// First element to enqueue	
	if(queue->size == 0){
		queue->front = new_data;
		queue->end = new_data;
	}
	else if(queue->size > 0){
		// Current last element shall point to the new last element
		queue->end->next = element
		// Set queue end pointer to the new last element
		queue->end = element;
	}

	++(queue->size);
	
	pthread_mutex_unlock(queue->mutex);
}

// Overloading function to ensure data is not lost while beeing on the stack
void IG_Queue_put(IG_Queue* queue, IG_Data new_data) {
	new_pointer = (IG_Data*)malloc(sizeof(IG_Data));
	new_pointer->id = new_data.id;
	new_pointer->datatype = new_data.IG_Datatype;
	new_pointer->data = new_data.data;
	new_pointer->IG_DataTime = new_data.IG_DataTime;
	// Recall so we dont have to rewrite
	IG_Queue_put(queue, new_pointer);
}

ID_Data* IG_Queue_take(IG_Queue* queue){


	// Lock
	pthread_mutex_lock(queue->mutex);

	// Check if queue is empty???
	if(queue->size==0){
		// TODO
		return IG_DATA_EMPTY;
	}
	
	// Get pointer to data of first element in queue
	IG_Data* dequeued_data = queue->front;

	// Set front pointer of queue to the next element in the queue
	queue->front = queue->front->next;

	--(queue->size);

	pthread_mutex_unlock(queue->mutex);	

	return dequeue_data;

}

IG_Bool IG_Queue_isEmpty(IG_Queue* queue){
	return (queue->size>0);
}






#include "queue.h"
#include "internals.h"

IG_Queue * IG_Queue_new(){

	// Init queue
	IG_Queue* queue = (IG_Queue*)malloc(sizeof(IG_Queue));
	queue->size = 0;
	queue->front = NULL;
	queue->end = NULL;
	queue->mutex = malloc(sizeof(*(queue->mutex)));

	// Init mutex for this specific queue
	pthread_mutex_init(queue->mutex,NULL);
	return queue;
}

void IG_Queue_delete(IG_Queue * queue){
	pthread_mutex_lock(queue->mutex);
	while(!IG_Queue_isEmpty(queue)){
		IG_Data* element = IG_Queue_take(queue);
		free(element->data);		
		free(element);
		//IG_Data_delete(element);
	}
	pthread_mutex_unlock(queue->mutex);
	pthread_mutex_destroy(queue->mutex);
	free(queue->mutex);
	free(queue);
}

// Function to enqueue data
void IG_Queue_put(IG_Queue* queue, IG_Data* new_data){

	// Trying to lock this queue from other threads, if it's already locked this thread will
	// get locked and will be blocked until the mutex is unlocked by another thread
	pthread_mutex_lock(queue->mutex);

	// Creating new element for the queue
	IG_QElement* element = (IG_QElement*)malloc(sizeof(IG_QElement));

	element->data = new_data;
	element->next = NULL;

	// First element to enqueue	
	if(queue->size == 0){
		queue->front = element;
		queue->end = element;
	}
	else if(queue->size > 0){
		// Current last element shall point to the new last element
		queue->end->next = element;
		// Set queue end pointer to the new last element
		queue->end = element;
	}

	++(queue->size);
	
	pthread_mutex_unlock(queue->mutex);
}

// Overloading function to ensure data is not lost while beeing on the stack
void IG_Queue_put_copy(IG_Queue* queue, IG_Data new_data) {
	IG_Data * new_pointer = (IG_Data*)malloc(sizeof(IG_Data));
	new_pointer->id = new_data.id;
	new_pointer->datatype = new_data.datatype;
	new_pointer->data = new_data.data;
	new_pointer->timestamp = new_data.timestamp;
	// Recall so we dont have to rewrite
	IG_Queue_put(queue, new_pointer);
}

IG_Data* IG_Queue_take(IG_Queue* queue){


	// Lock
	pthread_mutex_lock(queue->mutex);

	// Check if queue is empty???
	if(queue->size==0){
		pthread_mutex_unlock(queue->mutex);	
		return &IG_DATA_EMPTY;
	}
	
	// Get pointer to first element in queue
	IG_QElement* dequeued_data = queue->front;
	// Set front pointer of queue to the next element in the queue
	queue->front = queue->front->next;

	--(queue->size);

	pthread_mutex_unlock(queue->mutex);	

	//take datapointer out of listelement
	IG_Data * data = dequeued_data->data;

	//free listelement
	free(dequeued_data);

	return data;

}

IG_Bool IG_Queue_isEmpty(IG_Queue* queue){
	return (queue->size==0);
}






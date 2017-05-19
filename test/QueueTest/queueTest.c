// Testing the queue on its functionality
// And if its also providing its features of being threadsafe

#include <pthread.h>
#include "internals.h"


#include "queue.h"
#include <stdio.h>


typedef struct threadArgs{
	IG_Queue* queue;
	char* threadName;
} threadArgs;

void* erzeugeDaten(void *);
void* leseDaten(void *);

int main(void){
	
	// #1 Try: Testing the basics queue functions with one thread
	// Creating queue
	printf("---------- 1 Thread ----------\n");
	printf("Erzeuge Queue\n");

	IG_Queue* queue = IG_Queue_new(IG_QUEUE_NONBLOCKING);
	if(queue==NULL) return EXIT_FAILURE;

	printf("Erzeugen erfolgreich\n");	

	printf("Füge Daten hinzu\n");
	// Create Data and insert it into the queue
	for(int i = 0; i<100; ++i){

		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		if(data==NULL) return EXIT_FAILURE;		

		data->id = i;
		data->datatype = IG_INT32;

		int* record = (int*)malloc(sizeof(int));
		if(record==NULL) return EXIT_FAILURE;

		*(record) = i*i;		
		data->data = (void*)record;
		data->timestamp = 0;
		// Inserting into the queue
		IG_Queue_put(queue,data);
	}

	printf("Lese Daten aus\n");
	// Read the queue
	while(!IG_Queue_isEmpty(queue)){
		IG_Data* data = IG_Queue_take(queue);
		printf("ID: %d\n",data->id);
		printf("Datentyp: IG_INT\n");
		printf("Inhalt: %d\n",*((int*)(data->data)));
		printf("Time Stamp: %ld\n",data->timestamp);
		printf("-----------------\n\n");
	}
	
	printf("Zerstöre Queue\n");
	// Delete queue
	IG_Queue_delete(queue);
	if(queue!=NULL) return EXIT_FAILURE;

	// #2 Try:  Testing the basics queue functions with two threads

	// One Thread for writing and reading
	pthread_t threadWrite, threadRead;
	
	// Create args struct for pthread
	printf("Erzeuge Queue\n");
	queue = IG_Queue_new(IG_QUEUE_BLOCKING);
	if(queue==NULL) return EXIT_FAILURE;
	printf("Erzeugen erfolgreich\n");	


	// Create threads
	threadArgs argsW = (threadArgs){queue, "Writer"};
	threadArgs argsR = (threadArgs){queue, "Reader"};
	
	pthread_create(&threadWrite,NULL,&erzeugeDaten,(void*)&argsW);
	sleep(1);
	pthread_create(&threadRead,NULL,&leseDaten,(void*)&argsR);

	pthread_join(threadWrite,NULL);
	pthread_join(threadRead,NULL);

	IG_Queue_delete(queue);
	if(queue!=NULL) return EXIT_FAILURE;

	return 0;
}

// 2. Parameter um später anzugeben welcher Thread hier Daten hinzugefügt hat
void* erzeugeDaten(void* args){

	threadArgs* tArgs = (threadArgs*)args;

	for(int i = 0; i<100000; ++i){

		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		if(data==NULL) return EXIT_FAILURE;

		data->id = i;
		data->datatype = IG_INT32;		
		data->data = (void*)(tArgs->threadName);
		data->timestamp = 0;
		IG_Queue_put(tArgs->queue,data);
		printf("Daten hinzugefügt\n");
	}
}
void* leseDaten(void* args){

	threadArgs* tArgs = (threadArgs*)args;
	
	while(!IG_Queue_IsEmpty(tArgs->queue){
		IG_Data* data = IG_Queue_take(tArgs->queue);
		printf("ID: %d\n",data->id);
		printf("Inhalt: %s\n",(char*)(data->data));
		printf("Time Stamp: %ld\n",data->timestamp);
		printf("-----------------\n\n");
		free(data);
	}
}

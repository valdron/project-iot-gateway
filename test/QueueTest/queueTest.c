
#include <pthread.h>
#include "internals.h"


#include "queue.h"
#include <stdio.h>


typedef struct threadArgs{
	IG_Queue* queue;
	char* threadName;
} threadArgs;

void erzeugeDaten(IG_Queue *, char *);
void leseDaten(IG_Queue *);

int main(void){
	
	// #1 Versuch: Test der Queue Funktionen (1 Thread)
	// Erzuge Queue
	printf("---------- 1 Thread ----------\n");
	printf("Erzeuge Queue\n");
	IG_Queue* queue = IG_Queue_new(IG_QUEUE_NONBLOCKING);
	printf("Erzeugen erfolgreich\n");	

	printf("Füge Daten hinzu\n");
	// Erzeuge Daten in Füge in Queue ein
	for(int i = 0; i<100; ++i){
		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		data->id = i;
		data->datatype = IG_INT32;
		int* record = (int*)malloc(sizeof(int));
		*(record) = i*i;		
		data->data = (void*)record;
		data->timestamp = 0;
		// Einfügen in die Queue
		IG_Queue_put(queue,data);
	}

	printf("Lese Daten aus\n");
	// Auslesen
	while(!IG_Queue_isEmpty(queue)){
		IG_Data* data = IG_Queue_take(queue);
		printf("ID: %d\n",data->id);
		printf("Datentyp: IG_INT\n");
		printf("Inhalt: %d\n",*((int*)(data->data)));
		printf("Time Stamp: %ld\n",data->timestamp);
		printf("-----------------\n\n");
	}
	
	printf("Zerstöre Queue\n");
	IG_Queue_delete(queue);

	// #2 Versuch: Test der Queue Funktionen (2 Thread)

	// Einer liest, einer schreibt
	pthread_t threadWrite, threadRead;
	
	// Erzeuge Struct für Argumente
	threadArgs args = (threadArgs){queue, "Writer"};
	
	pthread_create(&threadWrite,NULL,&erzeugeDaten,(void*)&args);


	return 0;
}

// 2. Parameter um später anzugeben welcher Thread hier Daten hinzugefügt hat
void erzeugeDaten(IG_Queue * queue,char* string){
	for(int i = 0; i<100; ++i){
		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		data->id = i;
		data->datatype = IG_INT32;
		int* record = (int*)malloc(sizeof(int));
		record = string;		
		data->data = (void*)record;
		data->timestamp = 0;
		// Einfügen in die Queue
		IG_Queue_put(queue,data);
	}
}
void leseDaten(IG_Queue * queue){
	while(!IG_Queue_isEmpty(queue)){
		IG_Data* data = IG_Queue_take(queue);
		printf("ID: %d\n",data->id);
		printf("Datentyp: IG_INT\n");
		printf("Inhalt: %s\n",(char*)(data->data));
		printf("Time Stamp: %ld\n",data->timestamp);
		printf("-----------------\n\n");
	}
}

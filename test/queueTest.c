
#include<pthread.h>
#include "internals.h"


#include "queue.h"
#include <stdio.h>


typedef struct threadArgs{
	IG_Queue* queue;
	char* threadName;
} threadArgs;

void* erzeugeDaten(void*);
void* leseDaten(void*);

int main(void){
	
	// #1 Versuch: Test der Queue Funktionen (1 Thread)
	// Erzuge Queue
	printf("---------- 1 Thread ----------\n");
	printf("Erzeuge Queue\n");
	IG_Queue* queue = IG_Queue_new();
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
	printf("Erfolgreich zerstört\n");

	// #2 Versuch: Test der Queue Funktionen (2 Thread)

	printf("Erzeuge Queue\n");
	queue = IG_Queue_new();
	printf("Erzeugen erfolgreich\n");	

	// Einer liest, einer schreibt
	pthread_t threadWrite, threadRead;
	
	// Erzeuge Struct für Argumente
	threadArgs argsW = (threadArgs){queue, "Writer"};
	threadArgs argsR = (threadArgs){queue, "Reader"};
	
	// pthread erzeugen
	pthread_create(&threadWrite,NULL,&erzeugeDaten,(void*)&argsW);
	pthread_create(&threadRead,NULL,&leseDaten,(void*)&argsR);

	// Warten bis alle Daten eingetragen und ausgelesen wurden
	pthread_join(threadWrite,NULL);
	pthread_join(threadRead,NULL);

	sleep(10);
	return 0;
}

// 2. Parameter um später anzugeben welcher Thread hier Daten hinzugefügt hat
void* erzeugeDaten(void* args){
	threadArgs* argReader = (threadArgs*)args;
	IG_Queue* queue = argReader->queue;
	char* record = argReader->threadName;
	printf("Thread: %s gestartet\n",record);
	for(int i = 0; i<100; ++i){
		printf("Thread: %s fügt Paket mit ID: %d ein\n",record, i);
		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		data->id = i;
		data->datatype = IG_INT32;		
		data->data = (void*)record;
		data->timestamp = 0;
		// Einfügen in die Queue
		IG_Queue_put(queue,data);
	}
}
void* leseDaten(void* args){
	threadArgs* argReader = (threadArgs*)args;
	IG_Queue* queue = argReader->queue;
	char* text = argReader->threadName;
	while(!IG_Queue_isEmpty(queue)){
		printf("Current thread: %s\n",text);
		IG_Data* data = IG_Queue_take(queue);
		printf("ID: %d\n",data->id);
		printf("Datentyp: IG_INT\n");
		printf("Inhalt: %s\n",(char*)(data->data));
		printf("Time Stamp: %ld\n",data->timestamp);
		printf("-----------------\n\n");
	}
}

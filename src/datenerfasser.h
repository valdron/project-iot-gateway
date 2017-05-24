
#ifndef DATENERFASSER_H
#define DATENERFASSER_H

#include "queue.h"
#include "datenerfasser.h"
//for datatypes
#include "internals.h"
#include "configuration.h"




typedef struct{
    IG_Queue * queue;
    IG_Config * config;
    pthread_t erfasserThread;
} IG_Datenerfasser;


// returns next Data or returns IG_DATA_EMPTY;
// fetches the data from queue
IG_Data getNextData(IG_Datenerfasser * erfasser);

// returns if there is any new data in the queue
// should be called before getNextData();
bool anyNewData(IG_Datenerfasser * erfasser);


// initializes the connection to the Datasource(OPC UA) and starts the extra thread with loop over incoming data
// TODO: needs config parameters (as pointer?) 
// TODO: parameter for flags?
IG_Status init_erfasser(IG_Datenerfasser * erfasser);

//Allocates new IG_Datenerfasser struct on the heap
IG_Datenerfasser * IG_Datenerfasser_create_nonBlocking(IG_Config * config);


//Frees the memory needed for the struct including the queue
void IG_Datenerfasser_delete(IG_Datenerfasser * erfasser);

#endif
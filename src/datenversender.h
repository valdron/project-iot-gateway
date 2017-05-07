#ifndef DATENVERSENDER_H
#define DATENVERSENDER_H

//for datatypes
#include "queue.h"
#include "internals.h"
#include "configuration.h"
#include "pthread.h" // OVERWATCH ** Dopplung in queue.h --> maybe should be added in  "internals.h"

typedef struct{
    IG_Config * config;
    IG_Queue * queue;
} IG_Datenversender;



// gives the IG_Data to the datenversender thread (pushes it to the queue)
IG_Status sendData(IG_Datenversender * sender, IG_Data * data);


// returns next Data or returns IG_DATA_EMPTY;
// fetches the data from queue
IG_Data getNextData(IG_Datenversender * sender);

// returns if there is any new data in the queue
// should be called before getNextData();
bool anyNewData(IG_Datenversender * sender);


// initializes the connection to the Datadestination(MQTT-Broker) and starts the extra thread with loop over incoming data
// TODO: needs config parameters (as pointer?) 
// TODO: parameter for flags?
IG_Status init_versender(IG_Datenversender * sender);



//creates new struct on the heap and initializes it with the parameters
IG_Datenversender * IG_Datenversender_create(IG_Config * config);

// frees the memory needed for the struct
void IG_Datenversender_delete(IG_Datenversender * sender);

#endif
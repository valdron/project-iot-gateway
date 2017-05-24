#ifndef DATENVERSENDER_H
#define DATENVERSENDER_H

//for datatypes
#include "queue.h"
#include "internals.h"
#include "configuration.h"
#include "mqtt.h"

typedef struct{
    IG_Config * config;
    IG_Queue * queue;
} IG_Datenversender;



// gives the IG_Data to the datenversender thread (pushes it to the queue)
IG_Status sendData(IG_Datenversender * sender, IG_Data * data);


// Callback-Function Thread
void* doSomeThing(void *arg);


// initializes the connection to the Datadestination(MQTT-Broker) and starts the extra thread with loop over incoming data
// TODO: needs config parameters (as pointer?) 
// TODO: parameter for flags?
IG_Status init_versender(IG_Datenversender * sender);



//creates new struct on the heap and initializes it with the parameters
IG_Datenversender * IG_Datenversender_create(IG_Config * config);

// frees the memory needed for the struct
void IG_Datenversender_delete(IG_Datenversender * sender);

#endif
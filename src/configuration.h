#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "internals.h"
//TODO: Functions for reading from file and creating config structs

//describes what kind of Config it is
typedef enum {STANDARD, VERARBEITER, OPC, MQTT } IG_ConfigType;

//Describes what is in the dataptr of the ConfigRequest
typedef enum { UA_NODEID, IG_ID_REQ, NONE } IG_ConfigRequestDataType;

//Describes what kind of information is expected as a Result in the ConfigResponse and what really is in the ConfigResponse
typedef enum { IG_ID_RES, MQTT_TOPIC, OPC_SUBSCRIPTIONS, VERARBEITUNG, OPC_CONFIG, MQTT_CONFIG, VERARBEITER_CONFIG } IG_ConfigResponseDataType;

//TODO: Design config structs

typedef struct{
    IG_ConfigType type;
    // xmlDoc ptr
    /*...*/
} IG_Config;

typedef struct{
    /*...*/
    IG_ConfigRequestDataType requestType;
    IG_ConfigResponseDataType requestedResponsType;
    void * data;
} IG_ConfigRequest;

typedef struct{
    IG_Bool successful;
    IG_ConfigResponseDataType responseType;
    IG_UInt32 responseAmount;
    void * data;
    /*...*/
} IG_ConfigResponse;

// generic config request function
void get_config(IG_Config * config, IG_ConfigRequest * request, IG_ConfigResponse * response);


#endif
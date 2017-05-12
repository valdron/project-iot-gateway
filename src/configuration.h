#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include<libxml/tree.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>
#include<libxml/parser.h>
#include "internals.h"

//defines the xml rootnodenames for each config
#define STANDARD_ROOT_NODE_NAME "igconfig"
#define OPC_ROOT_NODE_NAME "opcconfig"
#define MQTT_ROOT_NODE_NAME "mqttconfig"
#define VERARBEITER_ROOT_NODE_NAME "verarbeiterconfig"

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
    xmlDocPtr ptr;
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
IG_Status IG_Config_get(IG_Config * config, IG_ConfigRequest * request, IG_ConfigResponse * response);


//create config from filename and ConfigType
IG_Config * IG_Config_create(char * filename, IG_ConfigType type);

#endif
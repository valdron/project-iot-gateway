
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include<libxml/tree.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>
#include<libxml/parser.h>
#include "internals.h"

//defines the xml rootnodenames for each config
static const char * STANDARD_ROOT_NODE_NAME = "igconfig";
static const char * OPC_ROOT_NODE_NAME  = "opcconfig";
static const char * MQTT_ROOT_NODE_NAME = "mqttconfig";
static const char * VERARBEITER_ROOT_NODE_NAME = "verarbeiterconfig";





//TODO: Functions for reading from file and creating config structs

//describes what kind of Config it is
typedef enum {
    IG_CONFIG_STANDARD, 
    IG_CONFIG_VERARBEITER, 
    IG_CONFIG_OPC, 
    IG_CONFIG_MQTT 
    } IG_ConfigType;

//Describes what is in the dataptr of the ConfigRequest
/*typedef enum { 
    IG_CONFIG_REQT_UA_NODEID, 
    IG_CONFIG_REQT_IG_ID, 
    IG_CONFIG_REQT_NONE 
    } IG_ConfigRequestDataType;

//Describes what kind of information is expected as a Result in the ConfigResponse and what really is in the ConfigResponse
typedef enum { 
    IG_CONFIG_REST_IG_ID, 
    IG_CONFIG_REST_MQTT_TOPIC, 
    IG_CONFIG_REST_OPC_SUBSCRIPTIONS, 
    IG_CONFIG_REST_VERARBEITUNG, 
    IG_CONFIG_REST_OPC_CONFIG, 
    IG_CONFIG_REST_MQTT_CONFIG, 
    IG_CONFIG_REST_VERARBEITER_CONFIG 
    } IG_ConfigResponseDataType;
*/
//TODO: Design config structs

typedef struct{
    IG_ConfigType type;
    xmlDocPtr ptr;
    /*...*/
} IG_Config;

/*typedef struct{
    
    IG_ConfigRequestDataType requestType;
    IG_ConfigResponseDataType requestedResponsType;
    void * data;
} IG_ConfigRequest;
*/

typedef struct{
    IG_UInt32 responseAmount;
    void * data;
    /*...*/
} IG_ConfigResponse;

// generic config request function
//IG_Status IG_Config_get(IG_Config * config, IG_ConfigRequest * request, IG_ConfigResponse * response);


//create config from filename and ConfigType
IG_Config * IG_Config_create(char * filename, IG_ConfigType type);
IG_Config * IG_Config_create_str(const unsigned char * xml_str, IG_ConfigType type);

void IG_Config_delete(IG_Config * config);

//helper function should not be used directly
IG_Status IG_Config_get_node_attribute(IG_Config * config, const unsigned char * xpath, const unsigned char * attr, unsigned char ** attr_res);

#endif
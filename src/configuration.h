#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "internals.h"
//TODO: Functions for reading from file and creating config structs


//TODO: Design config structs

typedef struct{
    /*...*/
} IG_Config;

typedef struct{
    /*...*/
} IG_ConfigRequest;

typedef struct{
    /*...*/
} IG_ConfigResponse;

// generic config request function
void get_config(IG_Config * config, IG_ConfigRequest * request, IG_ConfigResponse * response);


#endif
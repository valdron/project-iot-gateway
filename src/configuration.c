
#include<string.h>
#include<stdio.h>
#include "internals.h"
#include "configuration.h"


IG_Status check_root_node(const unsigned char * root_name, IG_ConfigType type);

IG_Config * IG_Config_create(char * filename, IG_ConfigType type){

    xmlDocPtr doc;
    xmlNodePtr root;
    doc = xmlParseFile(filename);
    if(doc == NULL) {
        printf("could not parse file: %s\n", filename);
        return NULL;
    }

    root = xmlDocGetRootElement(doc);
    if(root == NULL){
        printf("file: %s has no root node\n", filename);
        xmlFreeDoc(doc);
        return NULL;
    }

    IG_Status rt = check_root_node(root->name,type);
    if(rt != IG_STATUS_GOOD) {
        printf("file: %s has wrong rootnodename (name: %s)",filename ,root->name);
        xmlFreeDoc(doc);
        return NULL;
    }

    IG_Config * new = (IG_Config *) malloc(sizeof(IG_Config));
    new->ptr = doc;
    new->type = type;
    

    return new;
}

IG_Status check_root_node(const unsigned char * root_name, IG_ConfigType type) {
    IG_Status rt = IG_STATUS_GOOD;
    char * req_name;
    switch(type) {
        case STANDARD:
            req_name = STANDARD_ROOT_NODE_NAME;
        break;

        case VERARBEITER:
            req_name = VERARBEITER_ROOT_NODE_NAME;
        break;

        case MQTT:
            req_name = MQTT_ROOT_NODE_NAME;
        break;

        case OPC:
            req_name = OPC_ROOT_NODE_NAME;
        break;
    }
    if(strcmp(req_name, root_name) != 0) 
        rt = IG_STATUS_BAD;

    return rt;
}
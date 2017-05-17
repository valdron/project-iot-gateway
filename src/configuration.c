
#include<string.h>
#include<stdio.h>
#include "configuration.h"

//HELPER FUNCS-----------------------------
IG_Status check_root_node(const unsigned char * root_name, IG_ConfigType type);
//HELPER FUNCS-----------------------------

IG_Status IG_Config_get_node_attribute(IG_Config * config, const unsigned char * xpath, const unsigned char * attr, unsigned char ** attr_res) {
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodePtr node;

    ctx = xmlXPathNewContext(config->ptr);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(xpath, ctx);
    if(obj == NULL) {
        xmlXPathFreeContext(ctx);
        printf("could not eval xpath string: %s\n", xpath);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        printf("no nodes in xpathobj\n");
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval->nodeNr != 1) {
        printf("more than one node returned using first one");
    }

    node = *obj->nodesetval->nodeTab;
    
    unsigned char * attr_value = xmlGetProp(node, attr);
    if(attr_value == NULL) {
        printf("properties missing\n");
        xmlXPathFreeObject(obj);
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    IG_UInt32 size = strlen(attr_value) + 1;
    *attr_res = (unsigned char *) malloc((size)* sizeof(unsigned char));
    strncpy(*attr_res, attr_value, size);
    return IG_STATUS_GOOD;
}


//-------------------------------------------------
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
//-------------------------------------------------
IG_Config * IG_Config_create_str(const unsigned char * xml_str, IG_ConfigType type) {
    xmlDocPtr doc;
    xmlNodePtr root;
    doc = xmlParseDoc(xml_str);
    if(doc == NULL) {
        printf("could not parse string\n");
        return NULL;
    }

    root = xmlDocGetRootElement(doc);
    if(root == NULL){
        printf("string has no root node\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    IG_Status rt = check_root_node(root->name,type);
    if(rt != IG_STATUS_GOOD) {
        printf("string has wrong rootnodename (name: %s)" ,root->name);
        xmlFreeDoc(doc);
        return NULL;
    }

    IG_Config * new = (IG_Config *) malloc(sizeof(IG_Config));
    new->ptr = doc;
    new->type = type;
    
    return new;
}




//-------------------------------------------------
void IG_Config_delete(IG_Config * config) {
    xmlFreeDoc(config->ptr);
    free(config);
}
//-------------------------------------------------


//-------------------------------------------------
IG_Status check_root_node(const unsigned char * root_name, IG_ConfigType type) {
    IG_Status rt = IG_STATUS_GOOD;
    const char * req_name;
    switch(type) {
        case IG_CONFIG_STANDARD:
            req_name = STANDARD_ROOT_NODE_NAME;
        break;

        case IG_CONFIG_VERARBEITER:
            req_name = VERARBEITER_ROOT_NODE_NAME;
        break;

        case IG_CONFIG_MQTT:
            req_name = MQTT_ROOT_NODE_NAME;
        break;

        case IG_CONFIG_OPC:
            req_name = OPC_ROOT_NODE_NAME;
        break;

        default:
            req_name = "";
        break;
    }
    if(strcmp(req_name, root_name) != 0) 
        rt = IG_STATUS_BAD;

    return rt;
}
//-------------------------------------------------

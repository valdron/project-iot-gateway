
#include<string.h>
#include<stdio.h>
#include "internals.h"
#include "configuration.h"

//HELPER FUNCS-----------------------------
IG_Status check_root_node(const unsigned char * root_name, IG_ConfigType type);
//char * gen_xpath_string(IG_ConfigRequestDataType in_type, IG_ConfigResponseDataType out_type, void * in_data);
//HELPER FUNCS-----------------------------


//-------------------------------------------------
/*IG_Status IG_Config_get(IG_Config * config, 
                        IG_ConfigRequest * request, 
                        IG_ConfigResponse * response) {
    
    IG_ConfigResponseDataType res_type;
    IG_ConfigRequestDataType input_type;
    void * return_data;
    void * input_data;
    char * attr_name;

    char * xpath_string;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodePtr node;

    //check if is right config and request is valid (needed?)
    res_type = request->requestedResponsType;
    input_type = request->requestType;
    input_data = request->data;
    
    //create xpath string (helperfunc)
    xpath_string = gen_xpath_string(input_type, res_type, input_data);
    if(xpath_string == NULL) {
        printf("could not create xpath string\n");
        return IG_STATUS_BAD;
    }

    //create new xpath context
    ctx = xmlXPathNewContext(config->ptr);
    if(ctx == NULL) {
        printf("could not create XPath context");
        return IG_STATUS_BAD;
    }

    //eval xpath_string
    obj = xmlXPathEvalExpression(xpath_string, ctx);
    if(obj == NULL) {
        printf("could not eval xpath string: %s", xpath_string);
        return IG_STATUS_BAD;
    }

    //check result
    if(check_xpath_obj(obj) != IG_STATUS_GOOD) {
        printf("got no result from xpath\n");
        return IG_STATUS_BAD;
    }

    //get attribute from result node
    char * response = xmlGetProp(attr_name);

    //generate Response
    response->successful = true;
    response->responseType = res_type;
    response->responseAmount = 1;
    response->data = return_data;

    return IG_STATUS_GOOD;

}
*///-------------------------------------------------

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
/*-------------------------------------------------

char * gen_xpath_string(IG_ConfigRequestDataType in_type, 
                        IG_ConfigResponseDataType out_type, 
                        void * in_data) {
    char * xpath_str = NULL;
    //switch over reqtype
    switch(out_type) {
        case IG_CONFIG_REST_IG_ID:
            if(in_type == IG_CONFIG_REQT_UA_NODEID) {
                // do stuff
            } else {
                printf("wrong in_type\n");
            }
        break;
        
        case IG_CONFIG_REST_MQTT_TOPIC:
            if(in_type == IG_CONFIG_REQT_IG_ID) {
                //dostuff
            } else {
                 printf("wrong in_type\n");            

            }

        break;
         
        case IG_CONFIG_REST_OPC_SUBSCRIPTIONS:
            if(in_type == IG_CONFIG_REQT_NONE) {
                //dostuff
            } else {
                 printf("wrong in_type\n");            

            }
        break;
         
        case IG_CONFIG_REST_VERARBEITUNG:
            if(in_type == IG_CONFIG_REQT_IG_ID) {
                //dostuff
            } else {
                 printf("wrong in_type\n");            

            }
        break;
         
        case IG_CONFIG_REST_OPC_CONFIG:
            if(in_type == IG_CONFIG_REQT_NONE) {
                //dostuff
            } else {
                 printf("wrong in_type\n");            

            }
        break;
         
        case IG_CONFIG_REST_MQTT_CONFIG:
            if(in_type == IG_CONFIG_REQT_NONE) {
                //dostuff
            } else {
                 printf("wrong in_type\n");            

            }
        break;
         
        case IG_CONFIG_REST_VERARBEITER_CONFIG:
            if(in_type == IG_CONFIG_REQT_NONE) {
                //dostuff
            } else {
                 printf("wrong in_type\n");            

            }
        break;
         
        default:
            printf("ResponseType not implemented\n");
        break;
    }

    return xpath_str;
    }
*/
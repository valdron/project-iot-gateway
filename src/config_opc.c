
#include "config_opc.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//HELPER FUNCS
IG_Status create_subscriptions_response(xmlXPathObjectPtr obj, IG_ConfigResponse * response);
IG_Status create_opc_items(xmlXPathObjectPtr obj, IG_UInt32 subid, IG_ConfigResponse * response);

//-------------------------
IG_Status IG_Config_OPC_get_subscriptions(IG_Config * config, IG_ConfigResponse * response) {
    xmlDocPtr doc;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    doc = config->ptr;
    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(IG_OPCSUBSCRIPTION_XPATH, ctx);
    if(obj == NULL) {
        xmlXPathFreeContext(ctx);
        printf("could not eval xpath string: %s\n", IG_OPCSUBSCRIPTION_XPATH);
        return IG_STATUS_BAD;
    }
    
    printf("calling create_subscription_response\n");
    IG_Status rt = create_subscriptions_response(obj, response);
    if(rt != IG_STATUS_GOOD) {
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        printf("subscriptions couldnt be parsed\n");
        return IG_STATUS_BAD;
    }

    xmlXPathFreeContext(ctx);
    xmlXPathFreeObject(obj);
    return IG_STATUS_GOOD;
}
//-------------------------


//-------------------------
IG_Status IG_Config_OPC_get_items(IG_Config * config, IG_ConfigResponse * response, IG_UInt32 subid) {    
    
    xmlDocPtr doc;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    doc = config->ptr;
    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }
    unsigned char str[100];
    snprintf(str, 100, IG_OPCITEM_XPATH, subid);

    obj = xmlXPathEvalExpression(str, ctx);
    if(obj == NULL) {
        xmlXPathFreeContext(ctx);
        printf("could not eval xpath string: %s\n", IG_OPCSUBSCRIPTION_XPATH);
        return IG_STATUS_BAD;
    }

    IG_Status rt = create_opc_items(obj, subid, response);
    if(rt != IG_STATUS_GOOD) {
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        printf("items couldnt be parsed\n");
        return IG_STATUS_BAD;
    }

    xmlXPathFreeContext(ctx);
    xmlXPathFreeObject(obj);
    return IG_STATUS_GOOD;

}
//-------------------------

IG_Status IG_Config_OPC_get_conn_string(IG_Config * config, IG_ConfigResponse * response){
    

    //get stringparts
    unsigned char * proto = NULL;// = xmlGetProp(node, IG_OPCSERVER_PROTOCOL_VARNAME);
    unsigned char * hostname = NULL;// = xmlGetProp(node, IG_OPCSERVER_HOST_VARNAME);
    unsigned char * port = NULL;// = xmlGetProp(node, IG_OPCSERVER_PORT_VARNAME);
    
    IG_Status rt1 = IG_Config_get_node_attribute(config, IG_OPCSERVER_XPATH, IG_OPCSERVER_PROTOCOL_VARNAME, &proto);
    IG_Status rt2 = IG_Config_get_node_attribute(config, IG_OPCSERVER_XPATH, IG_OPCSERVER_HOST_VARNAME, &hostname);
    IG_Status rt3 = IG_Config_get_node_attribute(config, IG_OPCSERVER_XPATH, IG_OPCSERVER_PORT_VARNAME, &port);
    
    if(rt1 != IG_STATUS_GOOD || rt2 != IG_STATUS_GOOD || rt3 != IG_STATUS_GOOD) {
        printf("some needed attributes are missing");
        return IG_STATUS_BAD;
    }

    //build string
    unsigned char * str = (unsigned char *) malloc(100*sizeof(unsigned char));
    snprintf(str, 100, IG_OPC_CONN_STRING_FORMAT, proto, hostname, port);
    
    response->responseAmount = 1;
    response->data = (void*) str;
    return IG_STATUS_GOOD;
}


//parse the xpath answer to real structs
IG_Status create_subscriptions_response(xmlXPathObjectPtr obj, IG_ConfigResponse * response) {
    xmlNodePtr curr_node = NULL;
    IG_UInt32 amount;
    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        printf("no nodes in xpathobj\n");
        return IG_STATUS_BAD;
    }
    amount = obj->nodesetval->nodeNr;
    
    IG_OPC_Subscription * subs = (IG_OPC_Subscription *) malloc( amount * sizeof(IG_OPC_Subscription));
    
    IG_UInt32 j = 0;
    for(IG_UInt32 i = 0;i < amount; i++) {
        curr_node = obj->nodesetval->nodeTab[i];
        unsigned char * curr_node_interval = xmlGetProp(curr_node, IG_CONFIG_OPC_INTERVALATTRNAME);
        if(curr_node_interval == NULL) {
            continue;
        }

        unsigned char * curr_node_subid = xmlGetProp(curr_node, IG_CONFIG_OPC_SUBID);
        if(curr_node_subid == NULL) {
            continue;
        }

        IG_UInt32 subid = atoi(curr_node_subid);
        if(subid == 0) {
            continue;
        }
        IG_UInt32 interval = atoi(curr_node_interval);
        if(interval == 0) {
            continue;
        }
        
        subs[j].subscription_id = subid;
        subs[j].timeintervalms = interval;
        j++;
    }

    response->data = (void *) subs;
    response->responseAmount = j;
    return IG_STATUS_GOOD;


}


IG_Status create_opc_items(xmlXPathObjectPtr obj, 
                           IG_UInt32 subid, 
                           IG_ConfigResponse * response){
    xmlNodePtr curr_node = NULL;
    IG_UInt32 amount;
    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        printf("no nodes in xpathobj\n");
        return IG_STATUS_BAD;
    }
    amount = obj->nodesetval->nodeNr;
    
    IG_OPC_Item * items = (IG_OPC_Item *) malloc( amount * sizeof(IG_OPC_Item));
    
    IG_UInt32 j = 0;
    for(IG_UInt32 i = 0;i < amount; i++) {
        curr_node = obj->nodesetval->nodeTab[i];
        unsigned char * curr_node_nodeid = xmlGetProp(curr_node, IG_CONFIG_OPC_NODEIDNAME);
        unsigned char * curr_node_igid = xmlGetProp(curr_node, IG_CONFIG_OPC_IGIDNAME);
        
        if(curr_node_nodeid == NULL || curr_node_igid == NULL ) {
            continue;
        }

        IG_Id igid = atoi(curr_node_igid);
        IG_UInt32 nodeid = atoi(curr_node_nodeid);
        if(igid == 0 || nodeid == 0) {
            continue;
        }

        items[j].to_sub_id = subid;
        items[j].nodeid = nodeid;
        items[j].internal_id = igid;
        j++;
    }

    response->data = (void *) items;
    response->responseAmount = j;
    return IG_STATUS_GOOD;

}

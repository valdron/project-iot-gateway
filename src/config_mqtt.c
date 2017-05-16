#include "config_mqtt.h"


IG_Status IG_Config_MQTT_get_ClientConfig(IG_Config * config, IG_ConfigResponse * response) {

    unsigned char * conn_string = NULL;
    unsigned char * client_name = NULL;
    IG_UInt32 qos_level;
    IG_Config_MQTT_ClientConfig * configres = (IG_Config_MQTT_ClientConfig*) malloc(sizeof(IG_Config_MQTT_ClientConfig));

    IG_Status rt;
    
    //---------conn string
    conn_string = (unsigned char *) malloc(100 * sizeof(unsigned char));
    rt = IG_Config_MQTT_get_conn_string(config->ptr,conn_string);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get conn_string\n");
    }
    
    
    //---------Clientname
    client_name = (unsigned char *) malloc(100 * sizeof(unsigned char));
    rt = IG_Config_MQTT_get_client_name(config->ptr,client_name);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get client_name\n");
    }

    
    //---------qoslevel
    rt = IG_Config_MQTT_get_qos_level(config->ptr,&qos_level);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get qoslevel\n");
    }

    configres->client_name = client_name;
    configres->conn_string = conn_string;
    configres->qol_level = qos_level;
    
    response->amount = 1;
    response->data = (void*) configres;

    return IG_STATUS_GOOD; 

}

//gets/builds the connectionstring and writes it (max 100 char) to the buffer
IG_Status IG_Config_MQTT_get_conn_string(xmlDocPtr doc, unsigned char * buffer) {
    unsigned char * proto = NULL;
    unsigned char * hostname = NULL;
    unsigned char * port = NULL;

    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodePtr node;

    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(IG_CONFIG_MQTT_BROKER_XPATH, ctx);
    if(obj == NULL) {
        xmlXPathFreeContext(ctx);
        printf("could not eval xpath string: %s\n", IG_CONFIG_MQTT_BROKER_XPATH);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        printf("no nodes in xpathobj\n");
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }
    
    node = *obj->nodesetval->nodeTab;

    proto = xmlGetProp(IG_CONFIG_MQTT_PROTOCOL_VARNAME, node);
    host = xmlGetProp(IG_CONFIG_MQTT_HOST_VARNAME, node);
    port = xmlGetProp(IG_CONFIG_MQTT_PORT_VARNAME, node);
    if(host == NULL || proto == NULL || port == NULL) {
        printf("properties missing\n");
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    snprintf(buffer, 100, IG_CONFI_MQTT_CONN_STRING_FORMAT, proto, host, port);
    return IG_STATUS_GOOD;
}

IG_Status IG_Config_MQTT_get_client_name(xmlDocPtr doc, unsigned char * buffer) {
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodePtr node;
    unsigned char * name = NULL;

    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(IG_CONFIG_MQTT_BROKER_XPATH, ctx);
    if(obj == NULL) {
        xmlXPathFreeContext(ctx);
        printf("could not eval xpath string: %s\n", IG_CONFIG_MQTT_CLIENT_XPATH);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        printf("no nodes in xpathobj\n");
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    node = *obj->nodesetval->nodeTab;

    name = xmlGetProp(IG_CONFIG_MQTT_CLIENTNAME_VARNAME, node);
    if(name == NULL) {
        printf("no prop: %s\n", IG_CONFIG_MQTT_CLIENTNAME_VARNAME);
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    strncpy(buffer,name,100);

    return IG_STATUS_GOOD;
}

IG_Status IG_Config_MQTT_get_qos_level(xmlDocPtr doc, IG_UInt32 * qos) {
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodePtr node;
    unsigned char * qos_str = NULL;

    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(IG_CONFIG_MQTT_CLIENT_XPATH, ctx);
    if(obj == NULL) {
        xmlXPathFreeContext(ctx);
        printf("could not eval xpath string: %s\n", IG_CONFIG_MQTT_CLIENT_XPATH);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        printf("no nodes in xpathobj\n");
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        return IG_STATUS_BAD;
    }

    node = *obj->nodesetval->nodeTab;

    qos_str = xmlGetProp(IG_CONFIG_MQTT_QOS_VARNAME, node);
    if(qos_str == NULL) {
        printf("property not found\n");
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        return IG_STATUS_BAD;
    }
    *qos = atoi(qos_str);
    return IG_STATUS_GOOD;
}
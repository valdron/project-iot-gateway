#include "config_mqtt.h"
#include<string.h>


IG_Status IG_Config_MQTT_get_conn_string(IG_Config * config, unsigned char * buffer);
IG_Status IG_Config_MQTT_get_client_name(IG_Config * config, unsigned char * buffer);
IG_Status IG_Config_MQTT_get_qos_level(IG_Config * config, IG_UInt32 * qos);

//getting the appropriate topic for the id
IG_Status IG_Config_MQTT_get_topic(IG_Config * config, 
                              IG_Id id, 
                              IG_ConfigResponse * response) {

    unsigned char * topic_str = NULL;
    unsigned char xpath_str[100];

    //build xpath
    snprintf(xpath_str,100,IG_CONFIG_MQTT_TOPIC_FORMATSTRING,id);

    IG_Status rt;

    rt = IG_Config_get_node_attribute(config, xpath_str, IG_CONFIG_MQTT_TOPICNAME_VARNAME, &topic_str);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get attribute '%s' from xpath '%s'",IG_CONFIG_MQTT_TOPICNAME_VARNAME, xpath_str);
        return IG_STATUS_BAD;
    }

    response->responseAmount = 1;
    response->data = (void*) topic_str;

    return IG_STATUS_GOOD;

}


IG_Status IG_Config_MQTT_get_ClientConfig(IG_Config * config, IG_ConfigResponse * response) {

    unsigned char * conn_string = NULL;
    unsigned char * client_name = NULL;
    IG_UInt32 qos_level;
    IG_Config_MQTT_ClientConfig * configres = (IG_Config_MQTT_ClientConfig*) malloc(sizeof(IG_Config_MQTT_ClientConfig));

    IG_Status rt;
    
    //---------conn string
    conn_string = (unsigned char *) malloc(100 * sizeof(unsigned char));
    rt = IG_Config_MQTT_get_conn_string(config,conn_string);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get conn_string\n");
        free(conn_string);
        return IG_STATUS_BAD;
    }
    
    
    //---------Clientname
    client_name = (unsigned char *) malloc(100 * sizeof(unsigned char));
    rt = IG_Config_MQTT_get_client_name(config,client_name);
    if(rt != IG_STATUS_GOOD) {
        free(conn_string);
        free(client_name);
        printf("could not get client_name\n");
        return IG_STATUS_BAD;
    }

    
    //---------qoslevel
    rt = IG_Config_MQTT_get_qos_level(config,&qos_level);
    if(rt != IG_STATUS_GOOD) {
        free(conn_string);
        free(client_name);
        printf("could not get qoslevel\n");
        return IG_STATUS_BAD;
    }

    configres->client_name = client_name;
    configres->conn_string = conn_string;
    configres->qos_level =   qos_level;
    
    response->responseAmount = 1;
    response->data = (void*) configres;

    return IG_STATUS_GOOD; 

}

//gets/builds the connectionstring and writes it (max 100 char) to the buffer
IG_Status IG_Config_MQTT_get_conn_string(IG_Config * config, unsigned char * buffer) {
    unsigned char * proto = NULL;
    unsigned char * hostname = NULL;
    unsigned char * port = NULL;

    IG_Status rt1 = IG_Config_get_node_attribute(config, IG_CONFIG_MQTT_BROKER_XPATH, IG_CONFIG_MQTT_PROTOCOL_VARNAME, &proto);
    IG_Status rt2 = IG_Config_get_node_attribute(config, IG_CONFIG_MQTT_BROKER_XPATH, IG_CONFIG_MQTT_HOST_VARNAME, &hostname);
    IG_Status rt3 = IG_Config_get_node_attribute(config, IG_CONFIG_MQTT_BROKER_XPATH, IG_CONFIG_MQTT_PORT_VARNAME, &port);
    
    if(rt1 != IG_STATUS_GOOD || rt2 != IG_STATUS_GOOD || rt3 != IG_STATUS_GOOD) {
        printf("some needed attributes are missing");
        return IG_STATUS_BAD;
    }

   


    snprintf(buffer, 100, IG_CONFIG_MQTT_CONN_STRING_FORMAT, proto, hostname, port);
    return IG_STATUS_GOOD;
}

IG_Status IG_Config_MQTT_get_client_name(IG_Config * config, unsigned char * buffer) {
    
    unsigned char * name = NULL;
    IG_Status rt = IG_Config_get_node_attribute(config, IG_CONFIG_MQTT_CLIENT_XPATH, IG_CONFIG_MQTT_CLIENTNAME_VARNAME, &name);
    if(rt != IG_STATUS_GOOD) {
        printf("some needed attributes are missing");
        return IG_STATUS_BAD;
    }

    strncpy(buffer,name,100);

    return IG_STATUS_GOOD;
}

IG_Status IG_Config_MQTT_get_qos_level(IG_Config * config, IG_UInt32 * qos) {
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodePtr node;
    unsigned char * qos_str = NULL;

    IG_Status rt = IG_Config_get_node_attribute(config, IG_CONFIG_MQTT_CLIENT_XPATH, IG_CONFIG_MQTT_QOS_VARNAME, &qos_str);
    if(rt != IG_STATUS_GOOD) {
        printf("some needed attributes are missing");
        return IG_STATUS_BAD;
    } 
    
    *qos = atoi(qos_str);
    return IG_STATUS_GOOD;
}
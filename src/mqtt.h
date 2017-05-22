#ifndef MQTT_H
#define MQTT_H


#include "string.h"
#include "MQTTClient.h"
#include "config_mqtt.h"

typedef struct{
        // MQTT init
    MQTTClient client;
    MQTTClient_connectOptions conn_opts ;
    MQTTClient_message pubmsg;
    MQTTClient_deliveryToken token;

}IG_Mqtt;

// Duty of Message publishing
void pubmsg(MQTTClient_message pubmsg, char * payload, IG_Datenversender * sender);

// Disconnect the Client & Destroy it
void disconnect(MQTTClient * client);



#endif
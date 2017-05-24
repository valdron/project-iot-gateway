#ifndef MQTT_H
#define MQTT_H


#include "string.h"
#include "MQTTClient.h"
#include "config_mqtt.h"

typedef struct{
        // MQTT init
    MQTTClient client;
    IG_UInt32 qos_level;

}IG_Mqtt;


IG_Mqtt * IG_Mqtt_create();

// Duty of Message publishing
void pubmsg(IG_Mqtt * stack, char * payload, int len, char * topic, int timeout);

// Disconnect the Client & Destroy it
void disconnect(MQTTClient * client);



#endif
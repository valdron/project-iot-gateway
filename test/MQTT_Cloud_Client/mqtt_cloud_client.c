#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883" //address of the MQTT-Broker
#define CLIENTID    "CloudClient"
#define TOPIC       "#" //All topics in the MQTT-Broker
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

//Topics form the IOT-Gateway
//The following are testtopics!
#define Temperature "Roboterarm/Temperature"
#define Pressure    "Roboterarm/Pressure"
#define State       "Roboterarm/State"

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

                                                           //MQTTClient_message contains some flags and a payload void ptr <- check this out
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    //Check what is inside of the context ptr
    //Maybe a switch over the topicname -> 
    //remember only int as switch value
    int i;
    //char* payloadptr;

    if(!strcmp(topicName,Temperature)){
        printf("%s\t\tDouble\t\t%f",topicName,atof(*message->payload));
    }else if(!strcmp(topicName,Pressure)){
        printf("%s\t\tDouble\t\t%f",topicName,atof(*message->payload));
    }else if(!strcmp(topicName,State)){
        printf("%s\t\tInt\t\t%d",topicName,atoi(*message->payload));
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);	
    }

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);

    printf("Topic\t\tValuetype\t\tValue\n\n");

    MQTTClient_subscribe(client, TOPIC, QOS);

    do 
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
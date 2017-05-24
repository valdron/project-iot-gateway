#include "datenversender.h"

const static int timeout = 1000L;

// Struct for the thread-callback-function-params
typedef struct {
     IG_Datenversender * sender;  
     IG_Data * data;
     IG_Mqtt *stack;
} readThreadParams;

IG_Datenversender * IG_Datenversender_create(IG_Config * config) {
    IG_Datenversender * sender = (IG_Datenversender *) malloc(sizeof(IG_Datenversender));
    sender->config = config;
    sender->queue = IG_Queue_new(IG_QUEUE_BLOCKING);
    return sender;
}

void IG_Datenversender_delete(IG_Datenversender * sender) {
    IG_Queue_delete(sender->queue);
    free(sender);
}


IG_Status sendData(IG_Datenversender * sender, IG_Data * data) {
// right data in to Queue

    IG_Queue_put(sender->queue,data);
 // TODO: ?
}

void* doSomeThing(void *arg)
{

  readThreadParams *params = arg;
  IG_Mqtt *stack = params->stack;
  IG_Datenversender * sender = params->sender;  
  //IG_Data * data = params->data;

            // Loop, when thread creation is succeeded
    while ( 1 )
     {
    //  delay safe!
    IG_Data * data = IG_Queue_take(sender->queue);
     

    IG_Status rt;
    IG_ConfigResponse res;
    rt = IG_Config_MQTT_get_topic(sender->config,data->id, &res );
    if(rt != IG_STATUS_GOOD) {
        continue;
    } 

    char * topic =  (char*) res.data;
    char * payload = (char*)data->data;
    int len = strlen(payload);
 
     // publish the message
    pubmsg(stack, payload,len, topic, timeout);
    free(topic);
    IG_Data_delete_members(data);
    IG_Data_delete(data);

}
   
}

IG_Status init_versender(IG_Datenversender * sender) {
/* All this value in config struct "sender" || ADDRESS "tcp://localhost:1883" || CLIENTID "ExampleClientPub" || TOPIC "MQTT Examples"  ||QOS 1 TIMEOUT 10000L*/
int rc;
int err;
pthread_t tid;
IG_Status rt;
IG_ConfigResponse res;
rt = IG_Config_MQTT_get_ClientConfig(sender->config, &res );

if(rt != IG_STATUS_GOOD) {
    return IG_STATUS_BAD;
}   
IG_Config_MQTT_ClientConfig * configptr =  (IG_Config_MQTT_ClientConfig*) res.data;


readThreadParams *readParams;
readParams = malloc(sizeof(readThreadParams));
readParams->sender = sender;                                                                                                     
readParams->stack = IG_Mqtt_create();
readParams->stack->qos_level = configptr->qos_level;


// Create the Client
MQTTClient_create(&stack->client, configptr.conn_string, configptr.client_name, MQTTCLIENT_PERSISTENCE_NONE, NULL);
stack->conn_opts.keepAliveInterval = 20;
stack->conn_opts.cleansession = 1;

// check for Conn_ACK
 if ((rc = MQTTClient_connect(stack->client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        return IG_STATUS_BAD;
    }

 // CallBackMethod Sending out
    err = pthread_create(&tid, NULL, &doSomeThing, readParams);
        if (err != 0) {
            printf("\ncan't create thread");
            disconnect(&(readParams->stack.client));
            return IG_STATUS_BAD;
        }
        else {
            printf("\n Thread created successfully\n");
       }
}



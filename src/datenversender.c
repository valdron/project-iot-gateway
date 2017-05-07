#include "datenversender.h"


IG_Datenversender * IG_Datenversender_create(IG_Config * config) {
    IG_Datenversender * sender = (IG_Datenversender *) malloc(sizeof(IG_Datenversender));
    sender->config = config;
    sender->queue = IG_Queue_new();
    return sender;
}

void IG_Datenversender_delete(IG_Datenversender * sender) {
    IG_Queue_delete(sender->queue);
    free(sender);
}

IG_Data getNextData(IG_Datenversender * sender){
    return sender->queue.IG_Queue_take(sender->queue);

}

bool checkForNewData(IG_Datenversender * sender){
    // if Queue isn't empty there is new Data !
    return IG_Queue_isEmpty(sender->queue);
}

IG_Status sendData(IG_Datenversender * sender, IG_Data * data) {
    if (checkForNewData(&sender)){
    return  IG_STATUS_BAD;
    }
    else {    
    pubmsg.payload = getNextData(&sender).getPayload();
    pubmsg.payloadlen = strlen(getNextData(&sender).getPayload());
    pubmsg.qos = sender->config.getQOS();
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, sender->config.getTopic(), &pubmsg, &token);
    rc = MQTTClient_waitForCompletion(client, token, sender->config.getTimeout());
    printf("Message with delivery token %d delivered\n", token);

    return IG_STATUS_GOOD;
    }

}

void* doSomeThing(void *arg, IG_Datenversender * sender,  IG_Data * data)
{
    
    
    sendData(&sender, &data);
   
}

IG_Status init_versender(IG_Datenversender * sender) {
/* All this value in config struct "sender" || ADDRESS "tcp://localhost:1883" || CLIENTID "ExampleClientPub" || TOPIC "MQTT Examples"  ||QOS 1 TIMEOUT 10000L*/

// MQTT init
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

int rc;
int err;
pthread_t tid[1];

// Create the Client
MQTTClient_create(&client, sender->config.getAddress(), sender->config.getClientID(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
conn_opts.keepAliveInterval = 20;
conn_opts.cleansession = 1;

// check for Conn_ACK
 if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

// Loop with thread
while (1) {
    // CallBackMethod Sending out
    err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0) {
            printf("\ncan't create thread :[%s]", strerror(err));
            break;
        }
        else {
            printf("\n Thread created successfully\n");
        }
   delay(100);     // 100 ms

}
MQTTClient_disconnect(client, 10000);
MQTTClient_destroy(&client);
}


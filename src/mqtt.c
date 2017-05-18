#include "mqtt.h"


IG_Mqtt * IG_Mqtt_create() {
    IG_Mqtt * stack = (IG_Mqtt *) malloc(sizeof(IG_Mqtt));
   stack->client;
   stack->conn_opts = MQTTClient_connectOptions_initializer;
   stack->pubmsg  = MQTTClient_message_initializer;
   stack->token;
    return stack;
}



void pubmsg(IG_Mqtt * stack, char * payload, IG_Datenversender * sender) {

    stack->pubmsg.payload = getPayload(payload);
    stack->pubmsg.payloadlen = strlen(getPayload(payload));
    stack->pubmsg.qos = sender->config.getQOS();
    stack->pubmsg.retained = 0;
    MQTTClient_publishMessage(stack->client, getTopic(sender->config), &(stack->pubmsg), &(stack->token));
    rc = MQTTClient_waitForCompletion(stack->client, stack->token, getTimeout(sender->config));
    printf("Message with delivery token %d delivered\n", stack->tokentoken);

}

void disconnect(MQTTClient * client) {
MQTTClient_disconnect(client, 10000);
MQTTClient_destroy(&client);
}


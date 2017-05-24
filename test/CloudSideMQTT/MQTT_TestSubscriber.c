
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <sys/types.h>
#include <unistd.h>

#define ADDRESS     "tcp://localhost:1883"
//#define CLIENTID    "ExampleClientSub"
//#define TOPIC       "MQTT Examples"
//#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;



	// Subscribing to all topics
char* TOPIC = "#";	
char* CLIENTID = "TestSubscriber";
	
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{	
    /* FOR WORKING WITH FILESYSTEM   
	FILE *fp; // 1
	fp = fopen("payload.txt", "a"); // 2
	FOR WORKING WITH FILESYSTEM      */

    int i;
    char* payloadptr;
	
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;

    /* FOR WORKING WITH FILESYSTEM   
    int results = fprintf(fp,"{%s : %s}\n",payloadptr,topicName); // 3
    if (results == EOF) 
	{/*Failed to write do error code here.*/
    /*
	}

    for(i=0; i<=message->payloadlen; i++)
    {	
	
        putchar(*payloadptr++);
	
    }
	
    putchar('\n');
	fclose(fp); // 4

    FOR WORKING WITH FILESYSTEM      */

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

// Setting the Params for Specific "TOPIC" in case -> testscenario not wanting all the Topic via "#"
void setParams(int argc, char ** argv){
	for (int i =1 ; i < argc; ++i) {
	if(strcmp("-t",argv[i]) == 0) {
		printf("topic:%s ",argv[++i]);
		TOPIC =argv[i];
	}
	if(strcmp("-c",argv[i]) == 0) {
		printf("client:%s ",argv[++i]);
		CLIENTID = argv[i];
	}	
   }
   puts("");
}

int main(int argc, char* argv[])
{

if (argc > 1) {
	setParams(argc,argv);
}	

    
	// Declaring params for the Subscriber Client
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    int rc;
    int ch;

    // Create the Client and set it up
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;


    // Setting the Callbacks
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    // Connecting to the Client
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
	
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
    do 
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    

    // Disco + Destroy
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

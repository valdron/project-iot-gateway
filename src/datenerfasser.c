//#include "queue.h"
#include "datenerfasser.h"
#include "opc.h"

IG_Datenerfasser * IG_Datenerfasser_create_nonBlocking(IG_Config * config) {
    IG_Datenerfasser * erfasser = (IG_Datenerfasser *) malloc(sizeof(IG_Datenerfasser));
    erfasser->config = config;
    erfasser->queue = IG_Queue_new(IG_QUEUE_NONBLOCKING);
    erfasser->running = true;
    return erfasser;
}


void IG_Datenerfasser_delete(IG_Datenerfasser * erfasser) {
    
    IG_Queue_delete(erfasser->queue);
    free(erfasser);
}

void IG_Datenerfasser_stop(IG_Datenerfasser * erfasser) {
    erfasser->running = false;
    printf("shutting down erfasser!\n");
    pthread_join(erfasser->erfasserThread, NULL);
    printf("erfasser shutdown complete\n");
}

IG_Status init_erfasser(IG_Datenerfasser * erfasser){

    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);

    /* Listing endpoints */
    UA_EndpointDescription* endpointArray = NULL;
    size_t endpointArraySize = 0;

    IG_ConfigResponse res;
    if(IG_Config_OPC_get_conn_string(erfasser->config,&res) != IG_STATUS_GOOD){
        return IG_STATUS_BAD;
    }

    UA_StatusCode retval = UA_Client_getEndpoints(client, (char*)res.data,
                                                &endpointArraySize, &endpointArray);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
        UA_Client_delete(client);
        return IG_STATUS_BAD;
    }
    
    printf("%i endpoints found\n", (int)endpointArraySize);
   
    /*for(size_t i=0;i<endpointArraySize;i++){
        printf("URL of endpoint %i is %s\n", (int)++i,
                endpointArray[i].endpointUrl.data);
    }*/

    UA_Array_delete(endpointArray,endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

    /* Connect to a server */
    /* anonymous connect would be: retval = UA_Client_connect(client, "opc.tcp://localhost:16664"); */
    retval = UA_Client_connect(client, (char*)res.data);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return IG_STATUS_BAD;
    }

    //Liste der Items die im Loop beobachtet werden sollen
    IG_OPC_Nodes * nodeStruct = (IG_OPC_Nodes*)malloc(sizeof(IG_OPC_Nodes));

    //FIXME: Init not ready TO DO 
    OPC_init(client, nodeStruct, erfasser);

    printf("\nInit done!\n\n");

    IG_Datenerfasser_threadparamerters * param = (IG_Datenerfasser_threadparamerters*)malloc(sizeof(IG_Datenerfasser_threadparamerters));
    param->erfasser = erfasser;
    param->client = client;
    param->nodes = nodeStruct;

    if(pthread_create(&erfasser->erfasserThread, NULL, (void*) start_OPC_Client_thread, param) != 0){
            printf("OPC Client konnte nicht gestartet werden\n");
            return IG_STATUS_BAD;
    } 

    return IG_STATUS_GOOD;
}

#include <open62541.h>
#include "init_xml.h"
#include "Client_v2.h"

//parameter ist vom Typ IG_Datenerfasser
void *start_OPC_Client_thread(void * parameter){
  UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
  /* Listing endpoints */
  UA_EndpointDescription* endpointArray = NULL;
  size_t endpointArraySize = 0;
  UA_StatusCode retval = UA_Client_getEndpoints(client, "opc.tcp://localhost:16664",
                                                &endpointArraySize, &endpointArray);
  if(retval != UA_STATUSCODE_GOOD) {
    UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
    UA_Client_delete(client);
    return NULL;
  }
  printf("%i endpoints found\n", (int)endpointArraySize);
  for(size_t i=0;i<endpointArraySize;i++){
      printf("URL of endpoint %i is %.*s\n", (int)++i,
             (int)endpointArray[i].endpointUrl.length,
             endpointArray[i].endpointUrl.data);
  }

  UA_Array_delete(endpointArray,endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

  /* Connect to a server */
  /* anonymous connect would be: retval = UA_Client_connect(client, "opc.tcp://localhost:16664"); */
  retval = UA_Client_connect(client, "opc.tcp://localhost:16664");
  if(retval != UA_STATUSCODE_GOOD) {
      UA_Client_delete(client);
      return NULL;
  }

  //Liste der Items die im Loop beobachtet werden sollenS
  MonitoredItems *monitoredItems;
  IG_Datenerfasser *param = (IG_Datenerfasser*)parameter;
  //Init not ready TO DO
  init(client, monitoredItems, param->queue);

  printf("\nInit done!\n\n");

  //prototype testing init-function -> hardcoded examples
  //testinit(client);

  while (true){
    //Subscriptions werden ausgeführt/angefragt
    //Main loop des Clients
    UA_Client_Subscriptions_manuallySendPublishRequest(client);
  }  

  UA_Client_disconnect(client);
  UA_Client_delete(client);
}
	
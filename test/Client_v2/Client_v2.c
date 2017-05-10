#include <open62541.h>
#include "init_xml.h"
#include "response_handler.h"
#include "Client_v2.h"
#include <stdio.h>



int main(){
  UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
  /* Listing endpoints */
  UA_EndpointDescription* endpointArray = NULL;
  size_t endpointArraySize = 0;
  UA_StatusCode retval = UA_Client_getEndpoints(client, "opc.tcp://localhost:16664",
                                                &endpointArraySize, &endpointArray);
  if(retval != UA_STATUSCODE_GOOD) {
    UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
    UA_Client_delete(client);
    return (int)retval;
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
      return (int)retval;
  }

  //Liste der Items die im Loop beobachtet werden sollenS
  MonitoredItems *monitoredItems;
  //Init not ready TO DO
  init(client, monitoredItems);

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
  
  return (int) UA_STATUSCODE_GOOD;
}
	
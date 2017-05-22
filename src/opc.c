

#include <open62541.h>
#include <stdio.h>
#include "opc.h"
#include "config_opc.h"

IG_Status init(UA_Client * client, MonitoredItems * monitoredItems, IG_Datenerfasser * param);
void response_handler(UA_UInt32 monId, UA_DataValue *value, void *context);

//parameter ist vom Typ IG_Datenerfasser
void start_OPC_Client_thread(void * parameter){


//>>>>>>>>>>>>>>>>>>>>>>>>>>> VON HIER


  UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
  /* Listing endpoints */
  UA_EndpointDescription* endpointArray = NULL;
  size_t endpointArraySize = 0;
  UA_StatusCode retval = UA_Client_getEndpoints(client, "opc.tcp://localhost:16664",
                                                &endpointArraySize, &endpointArray);
  if(retval != UA_STATUSCODE_GOOD) {
    UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
    UA_Client_delete(client);
    return;
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
      return;
  }
  
  //Liste der Items die im Loop beobachtet werden sollenS
  MonitoredItems *monitoredItems;
  IG_Datenerfasser *param = (IG_Datenerfasser*)parameter;
  
  
  //FIXME: Init not ready TO DO 
  init(client, monitoredItems, param);

  printf("\nInit done!\n\n");

  //prototype testing init-function -> hardcoded examples
  //testinit(client);


  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BIS HIER sollte in der init_erfasser(...) sein
 
 
 
  while (true){
    //Subscriptions werden ausgeführt/angefragt
    //Main loop des Clients
    UA_Client_Subscriptions_manuallySendPublishRequest(client);
  }  

  UA_Client_disconnect(client);
  UA_Client_delete(client);
}
	


  IG_Status init(UA_Client * client, MonitoredItems * monitoredItems, IG_Datenerfasser * param) {
      IG_ConfigResponse res;
      IG_Status rt = IG_Config_OPC_get_subscriptions(param->config, &res);
      if(rt != IG_STATUS_GOOD) {
        return IG_STATUS_BAD;
      }
      IG_UInt32 subAmount = res.responseAmount;
      IG_OPC_Subscription * subs = res.data;

      for(IG_UInt32 i = 0; i < subAmount; i++ ) {

        IG_ConfigResponse res2;
        rt = IG_Config_OPC_get_items(param->config, &res2, subs[i].subscription_id);
        if(rt != IG_STATUS_GOOD) {
          return IG_STATUS_BAD;
        }
        IG_UInt32 itemAmount = res2.responseAmount;
        IG_OPC_Item * items = res2.data;
        
        //needs to be saved elsewhere (in Monitoreditemsstruct )
        IG_UInt32 subid;

        UA_Client_Subscriptions_new(client, UA_SubscriptionSettings_standard,&subid);

        for(IG_UInt32 i = 0; i < itemAmount; i++ ) {
          //needs to be stored elsewhere (in Monitored items struct)
          IG_UInt32 itemid;
          UA_NodeId id = UA_NODEID_NUMERIC(1,items[i].nodeid);
                                                                                            //vvv insert pointer to correct Monitored Item here
          UA_Client_Subscriptions_addMonitoredItem(client, subid, id, 0, &response_handler, (void*) &items[i], &itemid);
        }
      }
  }


  void response_handler(UA_UInt32 monId, UA_DataValue *value, void *context) {
    printf("hello");
  }
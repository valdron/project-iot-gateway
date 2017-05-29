

#include <open62541.h>
#include <stdio.h>
#include <stdlib.h>
#include "opc.h"
#include "config_opc.h"

//IG_Status OPC_init(UA_Client * client, IG_OPC_Nodes * monitoredItems, IG_Datenerfasser * param);
void response_handler(UA_UInt32 monId, UA_DataValue *value, void *context);

//parameter ist vom Typ IG_Datenerfasser
void start_OPC_Client_thread(void * param){
    printf("Erfasser Thread started!\n");
    UA_Client * client = ((IG_Datenerfasser_threadparamerters*) param)->client;
    IG_Datenerfasser * erfasser = ((IG_Datenerfasser_threadparamerters*) param)->erfasser;
    IG_OPC_Nodes * node_struct = ((IG_Datenerfasser_threadparamerters*) param)->nodes;
  while (1) {
    //Subscriptions werden ausgeführt/angefragt
    //Main loop des Clients
    if(!erfasser->running)
        break;
    UA_Client_Subscriptions_manuallySendPublishRequest(client);
  }  
  printf("shutting down erfasser!");

  UA_Client_disconnect(client);
  UA_Client_delete(client);
}


IG_Status OPC_init(UA_Client * client, IG_OPC_Nodes * nodeStruct, IG_Datenerfasser * param) {
      IG_ConfigResponse res;
      IG_Status rt = IG_Config_OPC_get_subscriptions(param->config, &res);
      if(rt != IG_STATUS_GOOD) {
        return IG_STATUS_BAD;
      }
      
      //Funktion für anzahl aller Items
      //monitoredItems = (monitoredItems*)malloc(sizeof(monitoredItems*) * anzahlItems());      
      IG_UInt32 subAmount = res.responseAmount;
      IG_OPC_Subscription * subs = res.data;
      IG_OPC_Nodes_init(nodeStruct, subAmount);
      nodeStruct->subscriptions = subs;

      for(IG_UInt32 i = 0; i < subAmount; i++ ) {

        IG_ConfigResponse res2;
        rt = IG_Config_OPC_get_items(param->config, &res2, subs[i].subscription_id);
        if(rt != IG_STATUS_GOOD) {
          return IG_STATUS_BAD;
        }
        IG_UInt32 itemAmount = res2.responseAmount;
        IG_OPC_Item * items = res2.data;
        nodeStruct->itemArray[i] = items;
        
        for(IG_UInt32 y = 0; y < itemAmount; y++){
            nodeStruct->itemArray[i][y].queue = param->queue;
        }

        nodeStruct->anzahlItemsArray[i] = itemAmount; 
        
        //needs to be saved elsewhere (in Monitoreditemsstruct)

        UA_Client_Subscriptions_new(client, UA_SubscriptionSettings_standard,&nodeStruct->subscriptions[i].OPC_subId);

        for(IG_UInt32 x = 0; x < itemAmount; x++ ) {

          UA_NodeId id = UA_NODEID_NUMERIC(1,items[x].nodeid);    

                                                                                                      
          UA_Client_Subscriptions_addMonitoredItem(client, nodeStruct->subscriptions[i].OPC_subId, id, UA_ATTRIBUTEID_VALUE,
                                 &response_handler, (void*) &items[x], &items[x].monId);
                                                    //vvv insert pointer to correct Monitored Item here
        }
      }
}


void response_handler(UA_UInt32 monId, UA_DataValue *value, void *newMonitoredItem) {
   
    IG_Data *data = IG_Data_create_empty();

    IG_OPC_Item *dataInfo = (IG_OPC_Item*) newMonitoredItem;
    
    if(value->value.data == NULL){
        printf("Datapointer ist NULL");
        return;
    }
    switch((intptr_t)value->value.type->typeIndex){
        case UA_TYPES_DOUBLE:
            data->datatype = IG_DOUBLE;
            break;
        case UA_TYPES_BYTE:
            data->datatype = IG_BYTE;//Change to Byte!!
            break;
        default: printf("Unknowen Type\n");
            break;
    }
    
    data->timestamp = value->sourceTimestamp;
    //UA_String sourceTimestamp = UA_DateTime_toString(value->sourceTimestamp);
    //printf("%.*s\n", (int)sourceTimestamp.length, sourceTimestamp.data);

    //zuweisung der Daten
    data->data = value->value.data;

    //Hier wird noch die NodeID als ID benutzt
    data->id = dataInfo->internal_id;

    //printf("ItemId:  %d\tWert:  %f   \t\n", data->id,*((double*)data->data));
    
    value->value.storageType = UA_VARIANT_DATA_NODELETE;
    //Schreibe data in Queue
    printf("Erfasser Thread writing data to queue!\n");

    IG_Queue_put(dataInfo->queue, data);
}


void IG_OPC_Nodes_init(IG_OPC_Nodes * Node_Struct, IG_UInt32 anzahlSubs){
    Node_Struct->anzahlSub = anzahlSubs;
    Node_Struct->anzahlItemsArray = (IG_UInt32*)malloc(sizeof(IG_UInt32) * anzahlSubs);
    Node_Struct->itemArray = (IG_OPC_Item**)malloc(sizeof(IG_OPC_Item*) * anzahlSubs);
}
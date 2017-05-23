
#ifndef CLIENT_V2_H
#define CLIENT_V2_H

#include <open62541.h>
#include <stdbool.h>
#include "queue.h"
#include "datenerfasser.h"
#include "config_opc.h"

typedef struct{
    IG_UInt32 anzahlSub;
    IG_OPC_Subscription *subscriptions;
    IG_UInt32 * anzahlItemsArray;
    IG_OPC_Item **itemArray;
}IG_OPC_Nodes;

//parameter ist vom Typ IG_Datenerfasser
void start_OPC_Client_thread(void * parameter);
void IG_OPC_Nodes_init(IG_OPC_Nodes *,IG_UInt32 anzahlSubs);
IG_Status OPC_init(UA_Client * client, IG_OPC_Nodes * nodeStruct, IG_Datenerfasser * param);

#endif
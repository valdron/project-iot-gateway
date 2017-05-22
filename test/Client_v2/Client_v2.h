
#ifndef CLIENT_V2_H
#define CLIENT_V2_H

#include "queue.h"
//#include <open62541.h>

typedef enum{
    ROBOTERARM_TEMPERATURE_DOUBLE_VALUE = 118,
    ROBOTERARM_PRESSURE_DOUBLE_VALUE = 119,
    ROBOTERARM_STATE_INT_VALUE = 120
}OPCItemType;

//To save data of the Items
typedef struct {
    UA_UInt32 subId;
    UA_UInt32 monId;
    OPCItemType type;
    IG_Queue * queue; 
    bool avgValue;
} MonitoredItems;

//parameter ist vom Typ IG_Datenerfasser
void *start_OPC_Client_thread(void * parameter);

#endif
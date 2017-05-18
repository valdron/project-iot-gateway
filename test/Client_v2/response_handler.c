
#include "response_handler.h"
#include "Client_v2.h"
#include "queue.h"
#include <stdio.h>

static char boolToChar(bool i){
    if(i){
        return '1';
    }
    return '0';
}

void handler_TheAnswerChanged(UA_UInt32 monId, UA_DataValue *value, void *structMonitoredItems) {
    IG_Data *data;
    data = (IG_Data*)malloc(sizeof(IG_Data));

    //DataValue->Variant->Datatype
    //value->value.type
    

    MonitoredItems *dataInfo = (MonitoredItems*) structMonitoredItems;
    
    if(value->value.data == NULL){
        printf("Datapointer ist NULL");
        return;
    }

    //data->timestamp = value->sourceTimestamp;
    //UA_String sourceTimestamp = UA_DateTime_toString(value->sourceTimestamp);
    //printf("%.*s\n", (int)sourceTimestamp.length, sourceTimestamp.data);

    //zuweisung der Daten
    data->data = value->value.data;

    //Hier wird noch die NodeID als ID benutzt
    data->id = dataInfo->itemtype;

    switch(dataInfo->itemtype){
        case ROBOTERARM_TEMPERATURE_DOUBLE_VALUE:
            data->datatype = IG_DOUBLE;
            data->itemtype = ROBOTERARM_TEMPERATURE_DOUBLE_VALUE;
            break;

        case ROBOTERARM_PRESSURE_DOUBLE_VALUE:
            data->datatype = IG_DOUBLE;
            data->itemtype = ROBOTERARM_PRESSURE_DOUBLE_VALUE
            break;

        case ROBOTERARM_STATE_INT_VALUE:
            data->datatype = IG_INT32;
            data->intemtype = ROBOTERARM_STATE_INT_VALUE;
            break;

        default: printf("Unbekannte ID\n");
            data->id = monId;
            break;
    }

    printf("ItemId:  %d\tWert:  %f   \tSubID   :%d\t\n", data->datatype,*((double*)data->data),dataInfo->subId);
    
    //Schreibe data in Queue
    IG_Queue_put(dataInfo->queue, data);
}
#include <open62541.h>
#include "internals.h"
#include "Client_v2.h"
#include <stdio.h>
#include <stdbool.h>

static char boolToChar(bool i){
    if(i){
        return '1';
    }
    return '0';
}

void handler_TheAnswerChanged(UA_UInt32 monId, UA_DataValue *value, void *structMonitoredItems) {
    IG_Data *data;
    data = (IG_Data*)malloc(sizeof(IG_Data));

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

    switch(dataInfo->type){
        case ROBOTERARM_TEMPERATURE_DOUBLE_VALUE:
            data->datatype = ROBOTERARM_TEMPERATURE_DOUBLE_VALUE;
            break;

        case ROBOTERARM_PRESSURE_DOUBLE_VALUE:
            data->datatype = ROBOTERARM_PRESSURE_DOUBLE_VALUE;
            break;

        default: printf("Unbekannte ID\n");
                 data->id = monId;
                 break;
    }

    if(dataInfo->avgValue == true){
        data->avgValue = true;
    }else{
        data->avgValue = false;
    }

    printf("ItemId:  %d\tWert:  %f   \tSubID   :%d\tDurchschnittswert    %c\n", data->datatype,*((double*)data->data),dataInfo->subId,boolToChar(data->avgValue));
    //SendeAnSpeicher(data);
}
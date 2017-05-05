#include "open62541.h"
#include "Internals.h"
#include <stdio.h>

void handler_TheAnswerChanged(UA_UInt32 monId, UA_DataValue *value, void *context) {
    IG_Data *data;
    data = (IG_Data*)malloc(sizeof(IG_Data));
    
    if(value->value.data == NULL){
        printf("Datapointer ist NULL");
        return;
    }

    //data->timestamp = value->sourceTimestamp;
    //UA_String sourceTimestamp = UA_DateTime_toString(value->sourceTimestamp);
    //printf("%.*s\n", (int)sourceTimestamp.length, sourceTimestamp.data);

    data->data = value->value.data;
    switch(monId){
        case ROBOTERARM_TEMPERATURE_DOUBLE_VALUE:
            data->datatype = ROBOTERARM_TEMPERATURE_DOUBLE_VALUE;
            data->id = ROBOTERARM_TEMPERATURE_DOUBLE_VALUE;
            break;

        case ROBOTERARM_PRESSURE_DOUBLE_VALUE:
            data->datatype = ROBOTERARM_PRESSURE_DOUBLE_VALUE;
            data->id = ROBOTERARM_PRESSURE_DOUBLE_VALUE;
            break;

        default: printf("Unbekannte ID\n");
                 data->id = monId;
                 break;
    }

    printf("ItemId:  %d     Wert:  %f\n", data->id,*((double*)data->data));
    //SendeAnSpeicher(data);
}
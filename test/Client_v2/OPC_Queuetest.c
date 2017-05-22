
#include "queue.h"
#include "../../internals.h"
#include "Client_v2.h"
#include <stdio.h>
#include <unistd.h>

void* reading_From_OPC_Queue(void * queue){
    
    IG_Data * data = IG_Data_create_empty();
    if(data == NULL){
        printf("data konnte nicht erstellt werden\n");
        return;
    }
    printf("IG_ID\t\tIG_Datatype\t\tDatavalue\t\t");

    while(1){
        data = IG_Queue_take((IG_Queue*) queue);
        if(data == NULL){
            continue;
        }           

        printf("%d\n", data->itemtype);

        switch(data->datatype){
            case ROBOTERARM_TEMPERATURE_DOUBLE_VALUE: printf("%d\t\t%d\t\t",data->id,data->datatype);
                    break;
            case ROBOTERARM_PRESSURE_DOUBLE_VALUE: printf("%d\t\t%d\t\t",data->id,data->datatype);
                    break;
            case ROBOTERARM_STATE_INT_VALUE: printf("%d\t\t%d\t\t",data->id, data->datatype);
                    break;

            default: printf("Datatype nicht bekannt oder Wert aus Queue leer\n");
        }
        //double *value = (double*)data->data;
        sleep(1);
    }
}
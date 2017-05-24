
#include "queue.h"
#include "internals.h"
#include "opc.h"
#include <stdio.h>
#include <unistd.h>

void reading_From_OPC_Queue(void * queue){

    IG_Data * data = IG_Data_create_empty();
    if(data == NULL){
        printf("data konnte nicht erstellt werden\n");
        return;
    }
    printf("IG_ID\t\tIG_Datatype\t\tDatavalue\t\t\n");

    while(1){
        data = IG_Queue_take((IG_Queue*) queue);
        if(data == NULL){
            continue;
        }    
        //FIXME: switch over ids instead

        switch(data->datatype){
            case IG_DOUBLE: 
                printf("%d\t\t%d\t\t\t%f\n",data->id, data->datatype,*((IG_Double*) data->data));
                break;
            
            case IG_BYTE:
                printf("%d\t\t%d\t\t\t%x\n",data->id, data->datatype,*((IG_Byte*) data->data));
                break;

            default: printf("Datatype nicht bekannt oder Wert aus Queue leer\n");
        }
        //double *value = (double*)data->data;
        sleep(1);
    }
}
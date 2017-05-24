

#include<pthread.h>
#include<stdio.h>

#include "internals.h"
#include "datenerfasser.h"
#include "OPC_Queuetest.h"
#include "opc.h"
#include "starter.h"
#include "config_opc.h"


int main(void) {
  
    pthread_t readfromOPCqueue;

    char * psydofilename = "abc.xml";
    //run Startet alle Threads und sendet, falls etwas nicht
    //funktioniert hat eine Fehlermeldung zurück

    IG_Datenerfasser * erfasser = IG_Datenerfasser_create_nonBlocking(IG_Config_create("opc_config.xml", IG_CONFIG_OPC));

    init_erfasser(erfasser);
    
    if(pthread_create(&readfromOPCqueue, NULL, (void*) reading_From_OPC_Queue, erfasser->queue) != 0){
        printf("ReadFromQueue Thread konnte nicht erstallt werden\n");
        return -1;
    }

    pthread_join(readfromOPCqueue,NULL);
}


#include<pthread.h>
#include<stdio.h>

#include "internals.h"
#include "datenerfasser.h"
#include "OPC_Queuetest.h"
#include "opc.h"
#include "config_opc.h"


int main(void) {
        //Starting OPC Client Thread
    //-> Struct als übergabe überlegen -> struct liegt in Client_v2.h
    // Übergabe der Addresse eines structs
    //-> Testweise wird NULL übergeben da config noch nicht implementiert ist  
    pthread_t OPC_Client_Thread;

    IG_Config * config = IG_Config_create("opc_config.xml", IG_CONFIG_OPC);
    //Erstelle duei Queue in die der OPC Cliet schreibt
    IG_Datenerfasser *erfasser = IG_Datenerfasser_create_nonBlocking(config);

    if(pthread_create(&OPC_Client_Thread, NULL, (void*) start_OPC_Client_thread, erfasser) != 0){
            printf("OPC Client konnte nicht gestartet werden\nProgramm wird beendet");
            return -1;
    } 
    
    pthread_t readfromOPCqueue;

    if(pthread_create(&readfromOPCqueue, NULL, (void *) reading_From_OPC_Queue, erfasser->queue) != 0){
            printf("OPC Client konnte nicht gestartet werden\nProgramm wird beendet");
            return -1;
    }

    pthread_join(OPC_Client_Thread,NULL);
    pthread_join(readfromOPCqueue,NULL);
}
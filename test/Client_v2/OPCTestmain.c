

#include<pthread.h>
#include<stdio.h>

#include "internals.h"
#include "datenerfasser.h"
#include "OPC_Queuetest.h"
#include "opc.h"
#include "starter.h"
#include "config_opc.h"


int main(void) {
    pthread_t OPC_Client_Thread;
    pthread_t readfromOPCqueue;

    char * psydofilename = "abc.xml";
    if(run(psydofilename, &OPC_Client_Thread, &readfromOPCqueue) != IG_STATUS_GOOD)
        printf("Starten des Gataways fehlgeschlagen\n");
        

    pthread_join(OPC_Client_Thread,NULL);
    pthread_join(readfromOPCqueue,NULL);
}
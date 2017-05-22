// main function
// argument parsing
// calling run(...)

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "Client_v2.h"
#include "OPC_Queuetest.h"
#include "datenerfasser.h"



//TODO: documentation about arguments

#define USAGE "USAGE: \nIoTGateway -f [config-file]\n"

int main(int argc, char * argv[]) {



    //Starting OPC Client Thread
    //-> Struct als übergabe überlegen -> struct liegt in Client_v2.h
    // Übergabe der Addresse eines structs
    //-> Testweise wird NULL übergeben da config noch nicht implementiert ist  
    pthread_t OPC_Client_Thread;

    //Erstelle duei Queue in die der OPC Cliet schreibt
    IG_Datenerfasser *erfasser = IG_Datenerfasser_create_nonBlocking(NULL);

    if(pthread_create(&OPC_Client_Thread, NULL, start_OPC_Client_thread,
         erfasser) != 0){
            printf("OPC Client konnte nicht gestartet werden\nProgramm wird beendet");
            return -1;
    } 
    
    pthread_t readfromOPCqueue;

    if(pthread_create(&readfromOPCqueue, NULL, reading_From_OPC_Queue,
            erfasser->queue) != 0){
            printf("OPC Client konnte nicht gestartet werden\nProgramm wird beendet");
            return -1;
    }
   
    // shutdown notice
    return 0;
}



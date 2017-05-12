// main function
// argument parsing
// calling run(...)

#include<stdio.h>
#include<string.h>
#include <pthread.h>
#include "Client_v2.h"

//TODO: documentation about arguments

#define USAGE "USAGE: \nIoTGateway -f [config-file]\n"

#include "starter.h"

int main(int argc, char * argv[]) {


    // parse & check arguments
    if(argc != 3) {
        printf(USAGE);
        return -1;
    }
    char * config_filename; 
    if(!strcmp("-f",argv[0])){
        printf(USAGE);
        return -1;
    } else {
        config_filename = argv[1];
    }

    // call run with parameters
    run(config_filename);

    //Starting OPC Client Thread
    //Client_v2.c muss in Thread umgeschreiben werden 
    //-> Struct als übergabe überlegen -> struct kommt dann in Client_v2.h
    pthread_t OPC_Client_Thread;
    int status_OPC_Client_Thread = pthread_create(&OPC_Client_Thread, NULL, start_OPC_Client_thread,
        IG_Datenerfasser_create_nonBlocking(NULL)); // Übergabe der Addresse eines structs
        //-> Testweise wird NULL übergeben da config noch nicht implementiert ist
    // shutdown notice
    
}




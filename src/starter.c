


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "starter.h"
#include "datenerfasser.h"
#include "datenversender.h"
#include "verarbeiter.h"

//COMMAND DEFINES
#define SHUTDOWN "shutdown"
#define HELP "help"
#define PRINT_HELP "commands: \nhelp: prints help\nshutdown: exits\n"
#define PROMPT "IG-Command Prompt ->"

void commandloop();

IG_Status run(char * filename, pthread_t *OPC_Client_Thread, pthread_t *readfromOPCqueue) {

    //read config from file

    
    //create structs
    //IG_Datenerfasser * erfasser = (IG_Datenerfasser *) malloc(sizeof(IG_Datenerfasser));
    IG_Datenerfasser * erfasser = IG_Datenerfasser_create_nonBlocking(IG_Config_create("opc_config.xml", IG_CONFIG_OPC));
    IG_Datenversender * sender = (IG_Datenversender *) malloc(sizeof(IG_Datenversender));
    IG_Verarbeiter * verarbeiter = (IG_Verarbeiter *) malloc(sizeof(IG_Verarbeiter));
    //init structs
    
    if(init_erfasser(erfasser, OPC_Client_Thread, readfromOPCqueue) != IG_STATUS_GOOD){
        printf("could not initialize erfasser\n");
        return IG_STATUS_BAD;
    }

    /*if (init(sender) != IG_STATUS_GOOD ) {
        printf("could not initialize sender");
        return IG_STATUS_BAD;
    }

    if (init(erfasser) != IG_STATUS_GOOD ) {
        printf("could not initialize erfasser");
        return IG_STATUS_BAD;
    }*/

    //start-thread


    // start loop
    commandloop();

    return IG_STATUS_GOOD;
    


}

void commandloop() {
    char commandbuffer[50]; 
    while(true) {
        printf(PROMPT);
        scanf("%s", commandbuffer);

        if(!strcmp(commandbuffer, SHUTDOWN) ) {
            break;
        } else if(!strcmp(commandbuffer, HELP) ) {
            printf(PRINT_HELP);
        } else {
            printf(PRINT_HELP);
        }
    }
}

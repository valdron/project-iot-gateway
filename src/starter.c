


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "starter.h"
#include "datenerfasser.h"
#include "datenversender.h"
#include "config_main.h"
#include "verarbeiter.h"

//COMMAND DEFINES
#define SHUTDOWN "shutdown"
#define HELP "help"
#define PRINT_HELP "commands: \nhelp: prints help\nshutdown: exits\n"
#define PROMPT "IG-Command Prompt ->"

void commandloop();

IG_Status run(char * filename) {

    //read config from file
    IG_Config * main_config = IG_Config_create(filename, IG_CONFIG_STANDARD);

    //get config filenames
    unsigned char * verarbeitercfg_filename = NULL;
    unsigned char * opccfg_filename = NULL;
    unsigned char * mqttcfg_filename = NULL;

    //get configs from filenames
    IG_ConfigResponse res;
    IG_Status rt;

    rt = IG_Config_Main_get_filename(main_config, &res, "verarbeiterconfig");
    if(rt != IG_STATUS_GOOD){
        printf("couldnt get config filename");
        return IG_STATUS_BAD;
    } 
    verarbeitercfg_filename = (unsigned char *)res.data;

    rt = IG_Config_Main_get_filename(main_config, &res, "mqttconfig");
    if(rt != IG_STATUS_GOOD){
        printf("couldnt get config filename");
        return IG_STATUS_BAD;
    } 
    mqttcfg_filename = (unsigned char *)res.data;

    rt = IG_Config_Main_get_filename(main_config, &res, "opcconfig");
    if(rt != IG_STATUS_GOOD){
        printf("couldnt get config filename");
        return IG_STATUS_BAD;
    } 
    opccfg_filename = (unsigned char *)res.data;

    IG_Config * verarbeitercfg = IG_Config_create(verarbeitercfg_filename, IG_CONFIG_VERARBEITER);
    IG_Config * erfassercfg    = IG_Config_create(opccfg_filename, IG_CONFIG_VERARBEITER);
    IG_Config * versendercfg   = IG_Config_create(mqttcfg_filename, IG_CONFIG_VERARBEITER);
    if(verarbeitercfg == NULL ||
       erfassercfg == NULL ||
       versendercfg == NULL) {
        printf("could not create config from file");
        return IG_STATUS_BAD;    
    }

    
    //create structs
    IG_Datenerfasser * erfasser = IG_Datenerfasser_create_nonBlocking(erfassercfg);
    IG_Datenversender * sender = IG_Datenversender_create(versendercfg);
    IG_Verarbeiter * verarbeiter = IG_Verarbeiter_create(verarbeitercfg, sender, erfasser);
    //init structs
    
    if(init_erfasser(erfasser) != IG_STATUS_GOOD){
        printf("could not initialize erfasser\n");
        return IG_STATUS_BAD;
    }

    if (init_erfasser(erfasser) != IG_STATUS_GOOD ) {
        printf("could not initialize erfasser");
        return IG_STATUS_BAD;
    }

    if (IG_verarbeiter_init(verarbeiter) != IG_STATUS_GOOD ) {
        printf("could not initialize verarbeiter");
    }

    //start-thread


    // start loop
    commandloop();

    IG_Datenerfasser_delete(erfasser);
    IG_Datenversender_delete(sender);
    IG_Verarbeiter_delete(verarbeiter);
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

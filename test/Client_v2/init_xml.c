
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "open62541.h"
#include "response_handler.h"

static UA_SubscriptionSettings getSubscriptonSettings(double time){
    UA_SubscriptionSettings * settings;
    settings = (UA_SubscriptionSettings*)malloc(sizeof(UA_SubscriptionSettings));
    settings->requestedPublishingInterval = (UA_Double)(time * 1000);
    settings->requestedLifetimeCount = 10000;
    settings->requestedMaxKeepAliveCount = 1;
    settings->maxNotificationsPerPublish=10;
    settings->publishingEnabled=true;
    settings->priority=0; 
    return *settings; 
}

static int printStringXml(char *txtptr){
    //Zeiger zeigt auf die Stelle nach dem Tag
    while (*txtptr != '\0' && *txtptr != '\n' && *txtptr != '<' && *txtptr != ' ')
    {
        printf("%c", *txtptr);
        txtptr += sizeof(char);
    }
    printf("\n");
}

static double getDoubleFromXml(char * txtptr){  
    //Zeiger zeigt auf die Stelle nach dem Tag
    char doubleBuffer[6];
    for(int i = 0; txtptr[i] != '<' && txtptr[i] != '\n' && 
                    txtptr[i] != EOF; i++){
            if (i >= 6){
                printf("ERROR time value in Xml ist to big. (To many digits)\n");
                return -1;
            }
            doubleBuffer[i] = txtptr[i];
        }
    doubleBuffer[5] = '\0';
    return atof(doubleBuffer);  
}

static int getNodeIdFromXml(char * txtptr){
    char idBuffer[3+1];
    for(int i = 0; txtptr[i] != '<' && txtptr[i] != '\n' && 
                    txtptr[i] != EOF; i++){
            if (i >= 3){
                printf("ERROR NodeId value in Xml is to high. (To many digits)\n");
                return -1;
            }
            idBuffer[i] = txtptr[i];
        }
    idBuffer[3+1] = '\0';
    return atoi(idBuffer);  
}

int init(UA_Client * client){
    printf("*********INIT STARTED*********");
    FILE *fp;
    long fileSize;
    char *xmlConfig;
    char *searchptr;

    //Definitionen für das durchsuchen des XML-files

    const char searchSubscriptions[] = "<subscriptions>";
    //const char seachSubscripionsEnd[] = "</subscriptions>";

    const char searchSubscription[] = "<subscription>";
    //const char searchSubscriptionEnd[] = "</subscription>";

    const char searchMonitoredItem[] = "<monitoredItem>";
    //const char searchMonitoredItemEnd[] = "</monitoredItem>";

    const char searchNodeID[] = "<nodeID>";
    //const char searchNodeIDEnd[] = "</nodeID>";

    const char searchTimeinterval[] = "<timeinterval>";
    //const char searchTimeintervalEnd[] = "</timeinterval>";

    const char searchDurchschnittswertBoolean[] = "<durchschnittswert>";
    //const char searchDurchschnittswertBooleanEnd[] = "</durchschnittswert>";

    const char searchPublishDurchschnittswert[] = "<publishDurchschnittswert>";
    //const char searchPublishDurchschnittswertEnd[] = "</publishDurchschnittswert>";

    fp = fopen("OPC_init.xml", "r");
    if(fp == NULL){
        printf("Datei nicht gefuden\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    xmlConfig = calloc(1, fileSize + sizeof(char));
    fread(xmlConfig, fileSize, 1, fp);
    fclose(fp);

    searchptr = &xmlConfig[0];
    

    for(int i = 0;searchptr != NULL; i++){
        if(searchptr == '\0'){
             printf("Ende des Files erreicht\n");
             return -1;
        }           
           
        if(searchptr = strstr(searchptr, searchSubscription)){
            searchptr += sizeof(searchSubscription) - sizeof(char);
            printf("\n\nSubscription:\t");
            printStringXml(searchptr);
            UA_UInt32 SubID = i;
            printf("SubID:\t%d\n", SubID);
            UA_UInt32 *monIDs;
            int idCounter = 1;
            
            while(searchptr = strstr(searchptr, searchMonitoredItem)){
                searchptr += sizeof(searchMonitoredItem) - sizeof(char);
                printf("\tMonitored Item:\t");
                printStringXml(searchptr);
                
                if(searchptr = strstr(searchptr, searchTimeinterval)){
                    searchptr += sizeof(searchTimeinterval) - sizeof(char);
                    printf("\tTimeintervall:\t%f\n",getDoubleFromXml(searchptr));
                    UA_Client_Subscriptions_new(client,
                        getSubscriptonSettings(getDoubleFromXml(searchptr)),&SubID);
                    //UA_Client_Subscriptions_new(...) ändert den Wert der SubID
                    //Deshalb wird neu zugewiesen
                    SubID = i;

                    if(searchptr = strstr(searchptr, searchNodeID)){
                        searchptr += sizeof(searchNodeID) - sizeof(char);   
                        //ItemID muss Unique sein. Jedes Item braucht seine eigene ID
                        idCounter++;
                        monIDs = realloc(monIDs, idCounter*sizeof(UA_UInt32));           
                        printf("\tNodeID:\t\t%d\n", getNodeIdFromXml(searchptr));
                        printf("\tSubID:\t\t%d\n", SubID);                       
                        UA_Client_Subscriptions_addMonitoredItem(client,SubID,
                            UA_NODEID_NUMERIC(1,getNodeIdFromXml(searchptr)),
                            UA_ATTRIBUTEID_VALUE, &handler_TheAnswerChanged,NULL, &monIDs[idCounter-1]);
                        
                    
                        if(searchptr = strstr(searchptr, searchDurchschnittswertBoolean)){
                            searchptr += sizeof(searchDurchschnittswertBoolean) - sizeof(char);
                            //48 ist der ASCII wert für die 0 aus dem XML file
                            if(!((int)searchptr[0] == 48))
                                printf("\tWert ist ein Druchschnittswert\n");
                            else
                                printf("\tWert wird immer geliefert\n");
                        }
                    }
                }
                //Prüfung ob die Subscription beendet ist oder noch 
                //mehr Items hinzugefügt werden müssen
                int x;
                int z;
                char * xptr = strstr(searchptr,searchSubscription);
                char * zptr = strstr(searchptr,searchMonitoredItem);
                
                for(x = 0;x<fileSize;x++){
                    if(xptr == &xmlConfig[x])
                    break;
                }

                for(z = 0;z<fileSize;z++){
                    if(zptr == &xmlConfig[z])
                    break;
                }

                if(x<z){
                     break;
                }else{
                    printf("\n");
                }                
            }
        }
    }
    return 0;
}
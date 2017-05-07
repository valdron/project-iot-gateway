
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "open62541.h"
#include "response_handler.h"
#include "Internals.h"

static UA_SubscriptionSettings getSubscriptonSettings(double time){
    //Kann durch parsen von XML erstzt werden -> einstellungen über XML
    // TODO readSettingsFromXml();
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

//Funktion geht von passender Position von txtptr aus
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

static int getNodeIdFromXml(char * txtptr,const char * searchNodeId, int verschiebung){
    if(txtptr = strstr(txtptr,searchNodeId)){
        txtptr += verschiebung;
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
    }else{
        printf("ERROR -> Tag nicht in XML gefunden\nWert kann nicht hunzigefügt werden");
        return -1;
    }  
}

static bool itemIsAvgValue(char * searchptr, const char * searchDurchschnittswertBoolean, int verschiebung){ 
    if(searchptr = strstr(searchptr, searchDurchschnittswertBoolean)){
        searchptr += verschiebung;
        //48 ist der ASCII wert für die 0 aus dem XML file
        if(!((int)searchptr[0] == 48)){
            printf("\tWert ist ein Druchschnittswert\n");
            return true;
        }else{
            printf("\tWert wird immer geliefert\n");
            return false;
        }                                
    }    
}

int init(UA_Client * client, MonitoredItems *monitoredItems){
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
        printf("Datei nicht gefuden oder konnte nicht geöffnet werden\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    xmlConfig = calloc(1, fileSize + sizeof(char));
    fread(xmlConfig, fileSize, 1, fp);
    fclose(fp);

    searchptr = &xmlConfig[0];

    //Get Count of the Monitored Items
    int countMonitoredItems = 0;
    while(searchptr = strstr(searchptr, searchMonitoredItem)){
        countMonitoredItems++;
        searchptr++;
    }
    //Allocate memory for the array of Monitired Items
    monitoredItems = (MonitoredItems*) calloc(countMonitoredItems, sizeof(MonitoredItems));
    //put read pointer back to start of te Xml-string
    searchptr = &xmlConfig[0];
    //Parsing the XML
    for(int i = 0;searchptr != NULL; i++){
        if(searchptr == '\0'){
             printf("Ende des Files erreicht\n");
             return -1;
        }           
           
        if(searchptr = strstr(searchptr, searchSubscription)){
            searchptr += sizeof(searchSubscription) - sizeof(char);
            printf("\n\nSubscription:\t");
            printStringXml(searchptr);
            monitoredItems[i].subId = i;
            printf("SubID:\t%d\n", monitoredItems[i].subId);

            if(searchptr = strstr(searchptr, searchTimeinterval)){
                searchptr += sizeof(searchTimeinterval) - sizeof(char);
                printf("Timeintervall:\t%f\n",getDoubleFromXml(searchptr));
                UA_Client_Subscriptions_new(client,getSubscriptonSettings(getDoubleFromXml(searchptr)),
                                    &monitoredItems[i].subId);
            }     
            
            for(int item = i;searchptr = strstr(searchptr, searchMonitoredItem);item++){
                searchptr += sizeof(searchMonitoredItem) - sizeof(char);
                printf("\tMonitored Item:\t");
                printStringXml(searchptr);                
                                                                  //SuchString muss als Parameter übergben werden                                             
                monitoredItems[item].avgValue = itemIsAvgValue(searchptr,searchDurchschnittswertBoolean,
                                                        sizeof(searchDurchschnittswertBoolean)-sizeof(char)); 
                //monId wird von OPC Stack zugewiesen -> nicht unique -> nicht damit arbeiten
                //Für diese zuweiseung (muss) die NodeId im itemType enum hinterlegt sein
                monitoredItems[item].type = getNodeIdFromXml(searchptr,searchNodeID,sizeof(searchNodeID)-sizeof(char)); //<- Funnktion gibt -1 zurück falls die NodeId nicht               
                UA_Client_Subscriptions_addMonitoredItem(client,monitoredItems[i].subId,                                //gefunden werden konnte in der XML
                    UA_NODEID_NUMERIC(1,getNodeIdFromXml(searchptr,searchNodeID,sizeof(searchNodeID)-sizeof(char))), 
                    UA_ATTRIBUTEID_VALUE, &handler_TheAnswerChanged,
                    (void*) &monitoredItems[item],  //<- context Paraeter in der Handlerfunktion
                    &(monitoredItems[item].monId)); //Ich halte im Item eine Referenz auf das
                                                    //Item "selber" (auf eine Struktur mit relevanten)
                                                    //Infos und kann in der Handlefunktion einfach auf
                                                    //das Item zugreifen und die daten auslesen
                                                    //Prüfung ob die Subscription beendet ist oder noch
                                                    //Typen müssen allerdings im enum stehen
                                                    //mehr Items hinzugefügt werden müssen
                                                    //Somit kann ich den Wert verarbeiten ohne die
                                                    //mońitoredItemID zu berücksichtigen welche nicht eindeutig ist                                                                               
                
                //Kopieren des Wertes des erstem Items
                //Addresse des erstn Items wird als Referenz für die
                //Subscription benutzt                                                
                monitoredItems[item].subId = monitoredItems[i].subId;
                printf("\tNodeID:\t\t%d\n", getNodeIdFromXml(searchptr, searchNodeID, sizeof(searchNodeID)-sizeof(char)));
                printf("\tSubID:\t\t%d\n", monitoredItems[item].subId); 
                            

                //Prüfe ob noch mehr Items zur Subscription gehören        
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
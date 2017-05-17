#include<stdio.h>
#include "configuration.h"
#include "config_opc.h"

#define FILENAME "opc_config.xml"

static const unsigned char * OPC_XML_STRING = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
                                               <opcconfig>\
                                                   <opcserver proto=\"opc.tcp\" hostname=\"localhost\" port=\"16664\"/>\
                                                   <subscription intervalms=\"1000\" subid=\"1\">\
                                                       <item nodeidnum=\"117\" igid=\"1\" name=\"Pressure\"/>\
                                                       <item nodeidnum=\"118\" igid=\"2\" name=\"Temperature\"/>\
                                                   </subscription>\
                                                   <subscription intervalms=\"500\" subid=\"2\">\
                                                       <item nodeidnum=\"119\" igid=\"3\" name=\"state\"/>\
                                                   </subscription>\
                                               </opcconfig>";

int main(void) {
    IG_Config * conf = IG_Config_create_str(OPC_XML_STRING, IG_CONFIG_OPC);

    if(conf == NULL) {
        printf("wrong format of xml\n");
        return -1;
    }
    IG_ConfigResponse res;

    IG_Status rt = IG_Config_OPC_get_conn_string(conf,&res);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get conn_string\n");
        return -1;
    } else {
        printf("CONNSTRING: '%s'\n",(unsigned char*) res.data);
    }

    rt = IG_Config_OPC_get_subscriptions(conf,&res);
    if(rt != IG_STATUS_GOOD) {
        return -1;
    }
    printf("could get subs\n");
    IG_OPC_Subscription * subs = (IG_OPC_Subscription *) res.data;
    printf("ResponseAmount=%i\n", res.responseAmount);
    for(unsigned int i = 0; i < res.responseAmount; i++) {
        printf("SUBNR:%i subid=%i, intervalms=%ims\n",i,subs[i].subscription_id, subs[i].timeintervalms);
        IG_ConfigResponse it;
        IG_Status status = IG_Config_OPC_get_items(conf,&it,subs[i].subscription_id);
        if(status != IG_STATUS_GOOD) {
            return -1;
            continue;
        }
        IG_OPC_Item * items = (IG_OPC_Item *) it.data;
        for(unsigned int j = 0; j < it.responseAmount; j++) {
            printf("\tsubid=%i, nodeid=%i, igid=%i\n",items[j].to_sub_id, items[j].nodeid, items[j].internal_id);
        }
    }

    
}
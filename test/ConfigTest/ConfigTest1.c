
#include<stdio.h>
#include "configuration.h"


static const unsigned char * MAIN_XML_STR = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
                                             <igconfig>\
                                                 <opcconfig filename=\"opc_config.xml\"/>\
                                                 <mqttconfig filename=\"mqtt_config.xml\"/>\
                                                 <verarbeiterconfig filename=\"verarbeiter_config.xml\"/>\
                                             </igconfig>";

int main(void) {
    IG_Config * conf = IG_Config_create_str(MAIN_XML_STR, IG_CONFIG_STANDARD);
    if(conf == NULL) {
        printf("couldnot parse string\n");
        return -1;
    }

    IG_Config_delete(conf);

    printf("everything worked!\n");
    return 0;
}
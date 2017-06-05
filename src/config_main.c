#include "config_main.h"
#include<string.h>

IG_Status IG_Config_Main_get_filename(IG_Config * config, IG_ConfigResponse * response, unsigned char * configtag) {

    unsigned char * filename;
    unsigned char xpath_str[20];

    

    snprintf(xpath_str, 20, IG_CONFIG_MAIN_XPATH_FORMAT, configtag);

    IG_Status rt = IG_Config_get_node_attribute(config, xpath_str, IG_CONFIG_MAIN_FILE_VARNAME, &filename);
    if(rt != IG_STATUS_GOOD) {
        printf("couldn't get nodeattribute\n");
        return IG_STATUS_BAD;
    }

    response->responseAmount = 1;
    response->data = (void*) filename;


    return IG_STATUS_GOOD;
}

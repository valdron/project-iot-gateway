
// Should Test the 'IG_Config_MQTT_get_topic' Function

#include<stdio.h>
#include "config_mqtt.h"

#define USAGE "command [xmlfile]"

int main(int argc, char * argv[]) {

    if(argc != 2) {
        printf(USAGE);
        return -1;
    }

    IG_Config * conf = IG_Config_create(argv[1] ,IG_CONFIG_MQTT);
    if(conf == NULL) {
        printf("wrong xml root node\n");
        return -1;
    }

    IG_ConfigResponse res;
    IG_Id id = 1;
    IG_Status rt = IG_Config_MQTT_get_topic(conf, id, &res);
    if(rt != IG_STATUS_GOOD) {
        printf("could not get topic for id:%i\n",id);
    }

    printf("ID=%i\tTopic='%s'\n", id, (unsigned char *)res.data);

    


    return 0;
}
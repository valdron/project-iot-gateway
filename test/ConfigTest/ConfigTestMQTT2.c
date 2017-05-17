
// Should Test the 'IG_Config_MQTT_get_topic' Function

#include<stdio.h>
#include "config_mqtt.h"

static const unsigned char * MQTT_XML_STRING = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
                                                <mqttconfig>\
                                                    <mqttbroker protocol=\"tcp\" hostname=\"localhost\" port=\"1883\"/>\
                                                    <mqttclient name=\"IoT-Gateway\" qos=\"0\"/>\
                                                    <topic igid=\"1\" string=\"/maschine1/temp/avglasthour\"/>\
                                                    <topic igid=\"2\" string=\"/maschine1/pressure/current\"/>\
                                                </mqttconfig>";


int main(void) {
    IG_Config * conf = IG_Config_create_str(MQTT_XML_STRING ,IG_CONFIG_MQTT);
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

// Should Test the 'IG_Config_MQTT_get_ClientConfig' Function

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

    IG_ConfigResponse mqttconfig_res;
    IG_Status rt = IG_Config_MQTT_get_ClientConfig(conf,&mqttconfig_res);
    if(rt != IG_STATUS_GOOD) {
        printf("ConfigRequest failed\n");
        return -1;
    }

    IG_Config_MQTT_ClientConfig * mqttconfig = (IG_Config_MQTT_ClientConfig*) mqttconfig_res.data;
    printf("MQTTClientConfig:\n\tconn_string=\t'%s'\n\tclient_name=\t'%s'\n\tqos_level=\t'%i'\n",
            mqttconfig->conn_string,
            mqttconfig->client_name, 
            mqttconfig->qos_level
            );


    return 0;
}


//XPATH STATICS

static const unsigned char * IG_CONFIG_MQTT_BROKER_XPATH = "/mqttconfig/mqttbroker";
static const unsigned char * IG_CONFIG_MQTT_CLIENT_XPATH = "/mqttconfig/mqttclient";

//STATIC FORMATSTRINGS
static const unsigned char * IG_CONFIG_MQTT_TOPIC_FORMATSTRING = "/mqttconfig/topic[@igid='%s']";
static const char * IG_CONFIG_MQTT_CONN_STRING_FORMAT = "%s://%s:%s";


//STATIC VARNAMES
static const unsigned char * IG_CONFIG_MQTT_HOST_VARNAME = "hostname";
static const unsigned char * IG_CONFIG_MQTT_PORT_VARNAME = "port";
static const unsigned char * IG_CONFIG_MQTT_PROTOCOL_VARNAME = "protocol";
static const unsigned char * IG_CONFIG_MQTT_CLIENTNAME_VARNAME = "name";
static const unsigned char * IG_CONFIG_MQTT_QOS_VARNAME = "qos";




typedef struct {
    unsigned char * conn_string;
    unsigned char * client_name;
    IG_UInt32       qos_level;
} IG_Config_MQTT_ClientConfig;

//getting the appropriate topic for the id
IG_Status IG_Config_MQTT_get_topic(IG_Config * config, 
                              IG_Id id, 
                              IG_ConfigResponse * response);

//returns the ClientConfig struct
IG_Status IG_Config_MQTT_get_ClientConfig(IG_Config * config, IG_ConfigResponse * response);




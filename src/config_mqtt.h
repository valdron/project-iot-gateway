



//getting the appropriate topic for the id
IG_Status IG_Config_MQTT_get_topic(IG_Config * config, 
                              IG_Id id, 
                              IG_ConfigResponse * response);

//returns the connection string to the mqtt broker
IG_Status IG_Config_MQTT_get_conn_string(IG_Config * config, IG_ConfigResponse * response);

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/MQTT_Cloud_Client)

ADD_EXECUTABLE(
    MQTT_Cloud_Client
    mqtt_cloud_client.c
    )
TARGET_LINK_LIBRARIES(MQTT_Cloud_Client pthread open62541 paho-mqtt3c) 
configure_file(${PROJECT_SOURCE_DIR}/test/MQTT_Cloud_Client/mqtt_CC_test.sh ${CMAKE_BINARY_DIR}/MQTT_Cloud_Client/mqtt_CC_test.sh COPYONLY)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ConfigTest)
ADD_EXECUTABLE(ConfigTest1 ConfigTest1.c ${PROJECT_SOURCE_DIR}/src/configuration.c)
TARGET_LINK_LIBRARIES(ConfigTest1 xml2)
configure_file(${PROJECT_SOURCE_DIR}/test_config_files/main_config.xml ${CMAKE_BINARY_DIR}/ConfigTest/main_config.xml COPYONLY)

ADD_EXECUTABLE(OPCConfigTest1 ConfigTestOPC.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_opc.c)
TARGET_LINK_LIBRARIES(OPCConfigTest1 xml2)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/opc_config.xml ${CMAKE_BINARY_DIR}/ConfigTest/opc_config.xml COPYONLY)

ADD_EXECUTABLE(MQTTConfigTest1 ConfigTestMQTT1.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_mqtt.c)
TARGET_LINK_LIBRARIES(MQTTConfigTest1 xml2)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/mqtt_config.xml ${CMAKE_BINARY_DIR}/ConfigTest/mqtt_config.xml COPYONLY)

ADD_EXECUTABLE(MQTTConfigTest2 ConfigTestMQTT2.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_mqtt.c)
TARGET_LINK_LIBRARIES(MQTTConfigTest2 xml2)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/mqtt_config.xml ${CMAKE_BINARY_DIR}/ConfigTest/mqtt_config.xml COPYONLY)
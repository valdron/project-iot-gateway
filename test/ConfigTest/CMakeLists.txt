set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ConfigTest)

#copy files to testlocation
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/main_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/mqtt_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mqtt_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/opc_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/opc_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/verarbeiter_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/verarbeiter_config.xml COPYONLY)


ADD_EXECUTABLE(ConfigTest1 ConfigTest1.c ${PROJECT_SOURCE_DIR}/src/configuration.c)
TARGET_LINK_LIBRARIES(ConfigTest1 xml2)
ADD_TEST(NAME ConfigTest1 COMMAND ConfigTest1 main_config.xml  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

ADD_EXECUTABLE(ConfigTestVerarbeiter1 ConfigTestVerarbeiter1.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_verarbeiter.c ${PROJECT_SOURCE_DIR}/src/internals.c)
TARGET_LINK_LIBRARIES(ConfigTestVerarbeiter1 xml2)
ADD_TEST(NAME ConfigTestVerarbeiter1 COMMAND ConfigTestVerarbeiter1 verarbeiter_config.xml  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

ADD_EXECUTABLE(OPCConfigTest1 ConfigTestOPC.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_opc.c)
TARGET_LINK_LIBRARIES(OPCConfigTest1 xml2)
ADD_TEST(NAME OPCConfigTest1 COMMAND OPCConfigTest1 opc_config.xml WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

ADD_EXECUTABLE(MQTTConfigTest1 ConfigTestMQTT1.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_mqtt.c)
TARGET_LINK_LIBRARIES(MQTTConfigTest1 xml2)
ADD_TEST(NAME MQTTConfigTest1 COMMAND MQTTConfigTest1 mqtt_config.xml WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

ADD_EXECUTABLE(MQTTConfigTest2 ConfigTestMQTT2.c ${PROJECT_SOURCE_DIR}/src/configuration.c ${PROJECT_SOURCE_DIR}/src/config_mqtt.c)
TARGET_LINK_LIBRARIES(MQTTConfigTest2 xml2)
ADD_TEST(NAME MQTTConfigTest2 COMMAND MQTTConfigTest2 mqtt_config.xml WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

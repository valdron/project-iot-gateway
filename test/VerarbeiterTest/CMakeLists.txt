set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/VerarbeiterTest)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/main_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/mqtt_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mqtt_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/opc_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/opc_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/verarbeiter_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/verarbeiter_config.xml COPYONLY)

ADD_EXECUTABLE(VerarbeiterTest1 verarbeiterTest1.c 
${PROJECT_SOURCE_DIR}/src/verarbeiter.c 
${PROJECT_SOURCE_DIR}/src/internals.c 
${PROJECT_SOURCE_DIR}/src/queue.c 
${PROJECT_SOURCE_DIR}/src/datenversender.c 
${PROJECT_SOURCE_DIR}/src/datenerfasser.c 
${PROJECT_SOURCE_DIR}/src/opc.c 
${PROJECT_SOURCE_DIR}/src/mqtt.c 
${PROJECT_SOURCE_DIR}/src/config_opc.c 
${PROJECT_SOURCE_DIR}/src/config_mqtt.c 
${PROJECT_SOURCE_DIR}/src/config_verarbeiter.c 
${PROJECT_SOURCE_DIR}/src/configuration.c)

TARGET_LINK_LIBRARIES(VerarbeiterTest1 xml2 pthread paho-mqtt3c open62541)

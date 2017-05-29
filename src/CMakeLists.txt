set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/main_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/mqtt_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mqtt_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/opc_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/opc_config.xml COPYONLY)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/test_config_files/verarbeiter_config.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/verarbeiter_config.xml COPYONLY)

SET(SOURCES_IG  main.c 
                starter.c 
                queue.c 
                datenerfasser.c 
                datenversender.c 
                verarbeiter.c 
                configuration.c
                config_main.c
                config_mqtt
                config_opc.c
                config_verarbeiter.c
                internals.c
                verarbeiter.c
                opc.c
                mqtt.c)

ADD_EXECUTABLE(IoTGateway ${SOURCES_IG})
TARGET_LINK_LIBRARIES(IoTGateway pthread open62541 xml2 paho-mqtt3c) #add mqtt + xml library
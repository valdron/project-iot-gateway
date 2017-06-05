set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


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
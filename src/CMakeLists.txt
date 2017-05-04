

ADD_EXECUTABLE(IoTGateway main.c starter.c queue.c datenerfasser.c datenversender.c verarbeiter.c configuration.c)
TARGET_LINK_LIBRARIES(IoTGateway pthread open62541) #add mqtt + xml library
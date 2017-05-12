set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
ADD_EXECUTABLE(IoTGateway main.c starter.c queue.c datenerfasser.c datenversender.c verarbeiter.c configuration.c)
TARGET_LINK_LIBRARIES(IoTGateway pthread open62541 xml2) #add mqtt + xml library
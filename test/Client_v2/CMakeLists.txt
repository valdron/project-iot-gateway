

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OPCTest)

ADD_EXECUTABLE(OPC_Client_test main.c Client_v2.c init_xml.c response_handler.c ${PROJECT_SOURCE_DIR}/src/internals.c ${PROJECT_SOURCE_DIR}/src/main.c)
TARGET_LINK_LIBRARIES(OPC_Client_test open62541)
configure_file(OPC_init.xml ${CMAKE_BINARY_DIR}/OPC_init.xml COPYONLY)

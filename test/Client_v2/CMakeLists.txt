

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

ADD_EXECUTABLE(Client_v2 Client_v2.c init_xml.c response_handler.c)
TARGET_LINK_LIBRARIES(Client_v2 open62541)

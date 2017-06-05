set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OPCTest)

ADD_EXECUTABLE(Server_v2 Server_v2.c RoboterarmObjekt_v2.c state.c)
TARGET_LINK_LIBRARIES(Server_v2 open62541 pthread)
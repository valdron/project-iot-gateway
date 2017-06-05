

project(IoTGateway)

include( CTest )


cmake_minimum_required(VERSION 3.0)

find_package(LibXml2 REQUIRED)
find_library(open62541_LIB open62541)
find_library(paho_LIB paho-mqtt3c)


if(NOT open62541_LIB)
    message(FATAL_ERROR "open62541 not found check readme")
else()
    message(STATUS "Found open62541")
endif()

if(NOT paho_LIB)
    message(FATAL_ERROR "paho not found check readme")
else()
    message(STATUS "Found paho")
endif()

# shortcut



INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src ${LIBXML2_INCLUDE_DIR})


#  makes tests
add_subdirectory(test)

#makes main program
add_subdirectory(src)



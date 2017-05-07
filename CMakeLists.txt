
project(IoTGateway)

cmake_minimum_required(VERSION 3.8)

find_package(libxml2)
find_library(open62541_LIB open62541)
find_library(paho_LIB paho-mqtt3a)


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



INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src ${libxml2_INCLUDE_DIRS})


#  makes tests
add_subdirectory(test)

#makes main program
add_subdirectory(src)



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#add opc-server
add_subdirectory(Client_v2)

add_subdirectory(Server_v2)

#add queue test
add_subdirectory(QueueTest)


add_subdirectory(ConfigTest)

#add mqtt Cloud Client_v2
add_subdirectory(MQTT_Cloud_Client)

add_subdirectory(InternalsTest)


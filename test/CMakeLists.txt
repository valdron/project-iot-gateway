set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


#add opc-server
add_subdirectory(Client_v2)

add_subdirectory(Server_v2)

#add queue test
ADD_EXECUTABLE(QueueTest queueTest.c ${SOURCE_DIR}/queue.c)
TARGET_LINK_LIBRARIES(QueueTest pthread)
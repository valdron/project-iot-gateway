set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


#add opc-server
add_subdirectory(opc-server)


#add queue test
ADD_EXECUTABLE(QueueTest queueTest.c ${SOURCE_DIR}/queue.c)
TARGET_LINK_LIBRARIES(QueueTest pthread)
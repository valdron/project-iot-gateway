set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/QueueTest)

ADD_EXECUTABLE(QueueTest1 queueTest.c ${PROJECT_SOURCE_DIR}/src/queue.c ${PROJECT_SOURCE_DIR}/src/internals.c)
TARGET_LINK_LIBRARIES(QueueTest1 pthread)

ADD_EXECUTABLE(QueueTest2 queueTest2.c ${PROJECT_SOURCE_DIR}/src/queue.c ${PROJECT_SOURCE_DIR}/src/internals.c)
TARGET_LINK_LIBRARIES(QueueTest2 pthread)

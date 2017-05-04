ADD_EXECUTABLE(QueueTest queue.c queueTest.c)
TARGET_LINK_LIBRARIES(QueueTest pthread open62541) #add mqtt + xml library

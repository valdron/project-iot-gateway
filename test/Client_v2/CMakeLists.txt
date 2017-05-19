

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OPCTest)

ADD_EXECUTABLE(
	OPC_Client
	Client_v2.c
	OPC_Test_Main.c
	OPC_Queuetest.c
	init_xml.c
	response_handler.c
	${PROJECT_SOURCE_DIR}/src/internals.c
	${PROJECT_SOURCE_DIR}/src/datenerfasser.c
	${PROJECT_SOURCE_DIR}/src/starter.c 
	${PROJECT_SOURCE_DIR}/src/queue.c
	)

TARGET_LINK_LIBRARIES(OPC_Client open62541 pthread)
configure_file(OPC_init.xml ${CMAKE_BINARY_DIR}/OPCTest/OPC_init.xml COPYONLY)

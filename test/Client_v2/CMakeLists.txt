

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OPCTest)

ADD_EXECUTABLE(
	OPC_Test 
	init_xml.c
	response_handler.c
	OPC_Queuetest.c
	OPCTestmain.c
	${PROJECT_SOURCE_DIR}/src/internals.c
	${PROJECT_SOURCE_DIR}/src/opc.c
	${PROJECT_SOURCE_DIR}/src/queue.c
	${PROJECT_SOURCE_DIR}/src/datenerfasser.c
	)

TARGET_LINK_LIBRARIES(OPC_Test open62541 pthread)
configure_file(OPC_init.xml ${CMAKE_BINARY_DIR}/OPCTest/OPC_init.xml COPYONLY)

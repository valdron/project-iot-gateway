

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OPCTest)

ADD_EXECUTABLE(
<<<<<<< HEAD
	OPC_Client
	Client_v2.c
	OPC_Test_Main.c
	OPC_Queuetest.c
	init_xml.c
	response_handler.c
	${PROJECT_SOURCE_DIR}/src/queue.c
	${PROJECT_SOURCE_DIR}/src/datenerfasser.c
	${PROJECT_SOURCE_DIR}/src/internals.c	
	)

TARGET_LINK_LIBRARIES(OPC_Client open62541 pthread)
configure_file(OPC_init.xml ${CMAKE_BINARY_DIR}/OPCTest/OPC_init.xml COPYONLY)
=======
	OPC_Test 
	OPC_Queuetest.c
	OPCTestmain.c
	${PROJECT_SOURCE_DIR}/src/internals.c
	${PROJECT_SOURCE_DIR}/src/opc.c
	${PROJECT_SOURCE_DIR}/src/queue.c
	${PROJECT_SOURCE_DIR}/src/datenerfasser.c
	${PROJECT_SOURCE_DIR}/src/config_opc.c
	${PROJECT_SOURCE_DIR}/src/configuration.c
	)

TARGET_LINK_LIBRARIES(OPC_Test open62541 pthread xml2)
configure_file(${PROJECT_SOURCE_DIR}/test_config_files/opc_config.xml ${CMAKE_BINARY_DIR}/OPCTest/opc_config.xml COPYONLY)
>>>>>>> 8132125b65226328d494d2a567867a2f4d1f479d

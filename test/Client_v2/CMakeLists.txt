

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OPCTest)

ADD_EXECUTABLE(
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

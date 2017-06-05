# CMake generated Testfile for 
# Source directory: /home/firat/IoTGateway/project-iot-gateway/test/ConfigTest
# Build directory: /home/firat/IoTGateway/project-iot-gateway/test/ConfigTest
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ConfigTest1 "/home/firat/IoTGateway/project-iot-gateway/ConfigTest/ConfigTest1" "main_config.xml")
set_tests_properties(ConfigTest1 PROPERTIES  WORKING_DIRECTORY "/home/firat/IoTGateway/project-iot-gateway/ConfigTest")
add_test(ConfigTestVerarbeiter1 "/home/firat/IoTGateway/project-iot-gateway/ConfigTest/ConfigTestVerarbeiter1" "verarbeiter_config.xml")
set_tests_properties(ConfigTestVerarbeiter1 PROPERTIES  WORKING_DIRECTORY "/home/firat/IoTGateway/project-iot-gateway/ConfigTest")
add_test(OPCConfigTest1 "/home/firat/IoTGateway/project-iot-gateway/ConfigTest/OPCConfigTest1" "opc_config.xml")
set_tests_properties(OPCConfigTest1 PROPERTIES  WORKING_DIRECTORY "/home/firat/IoTGateway/project-iot-gateway/ConfigTest")
add_test(MQTTConfigTest1 "/home/firat/IoTGateway/project-iot-gateway/ConfigTest/MQTTConfigTest1" "mqtt_config.xml")
set_tests_properties(MQTTConfigTest1 PROPERTIES  WORKING_DIRECTORY "/home/firat/IoTGateway/project-iot-gateway/ConfigTest")
add_test(MQTTConfigTest2 "/home/firat/IoTGateway/project-iot-gateway/ConfigTest/MQTTConfigTest2" "mqtt_config.xml")
set_tests_properties(MQTTConfigTest2 PROPERTIES  WORKING_DIRECTORY "/home/firat/IoTGateway/project-iot-gateway/ConfigTest")

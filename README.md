# project-iot-gateway
A Project of students of the Hochschule Esslingen

##Installing Dependencies##

###open62541###

```bash
git clone https://github.com/open62541/open62541.git
cd open62541
mkdir build
cd build
cmake ..
```
CMakeCache.txt bearbeiten   
UA\_ENABLE_AMALGAMATION:BOOL=FALSE   
ersetzen durch  
UA\_ENABLE_AMALGAMATION:BOOL=ON  
```bash
cmake ..
make install #evtl sudo make install
```

###paho_mqtt###

```bash
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
cmake .
make install #evtl sudo make install
```

##Creating CMakeLists.txt##
```cmake
INCLUDE_DIRECTORIES("." dir1 dir2)
ADD_EXECUTABLE(NAME file1.c file2.c file3.c ...)
TARGET\_LINK_LIBRARIES(TARGETNAME LIBRARY)
```




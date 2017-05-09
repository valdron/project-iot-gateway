# project-iot-gateway
A Project of students of the Hochschule Esslingen

## Installing Dependencies ##

### open62541 ###
Source: https://github.com/open62541/open62541 

LICENSE: Mozilla Public License 2.0

```bash
curl -LJO https://github.com/open62541/open62541/archive/0.2-rc2.tar.gz
tar -xvf open62541-0.2-rc2.tar.gz && cd open62541-0.2-rc2
mkdir build && cd build
cmake -DUA_ENABLE_AMALGAMATION=ON ..
make && sudo make install
```
### paho_mqtt ###
 Source: https://github.com/eclipse/paho.mqtt.c 
 
 LICENSE: Eclipse Distribution License - v 1.0
```bash
curl -LJO https://github.com/eclipse/paho.mqtt.c/archive/v1.1.0.tar.gz
tar -xvf paho.mqtt.c-1.1.0.tar.gz && cd paho.mqtt.c-1.1.0
cmake -DPAHO_BUILD_STATIC=TRUE .
make && sudo make install
```

### libxml2 ###

SOURCE: https://xmlsoft.org/libxml2 

LICENSE: MIT LICENSE

```bash
curl -o libxml2-2.9.4.tar.gz ftp://xmlsoft.org/libxml2/libxml2-2.9.4.tar.gz
tar -xvf libxml2-2.9.4.tar.gz
cd libxml2-2.9.4
./configure --enable-static=yes
make && sudo make install #may take some minutes
```

## Creating CMakeLists.txt ##
```cmake
INCLUDE_DIRECTORIES("." dir1 dir2)
ADD_EXECUTABLE(NAME file1.c file2.c file3.c ...)
TARGET_LINK_LIBRARIES(TARGETNAME LIBRARY)
```

## Building ##

```bash
mkdir build && cd build
cmake ..
make
```


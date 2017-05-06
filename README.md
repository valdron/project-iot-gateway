# project-iot-gateway
A Project of students of the Hochschule Esslingen

## Installing Dependencies ##

### open62541 ###

```bash
git clone https://github.com/open62541/open62541.git
cd open62541
mkdir build
cd build
cmake ..
```
CMakeCache.txt bearbeiten   
UA\_ENABLE_AMALGAMATION:BOOL=OFF   
ersetzen durch  
UA\_ENABLE_AMALGAMATION:BOOL=ON  
```bash
cmake ..
sudo make install
```

### paho_mqtt ###

```bash
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
cmake .
sudo make install
```

### libxml2 ###

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


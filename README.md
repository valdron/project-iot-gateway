# project-iot-gateway
A Project of students of the Hochschule Esslingen

**Installing Dependencies**

open62541:
```bash
git clone https://github.com/open62541/open62541.git
cd open62541
mkdir build
cd build
cmake ..
´´´
CMakeCache.txt bearbeiten 
UA\_ENABLE_AMALGAMATION:BOOL=FALSE 
ersetzen durch
UA_ENABLE_AMALGAMATION:BOOL=ON
```bash
cmake ..
make install #evtl sudo make install
´´´

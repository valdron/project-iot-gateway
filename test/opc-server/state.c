#include "open62541.h"
#include "state.h"

// State:
// 1 Byte 
// Bit 0: 1 = on; 0 = off
// Bit 1: 1 = in bewegung; 0 = still
// Bit 2: 1 = working; 0 = waiting
// Bit 3: 1 = fehler; 0 = kein fehler

// reserved
// Bit 4: 1 = switching aufsatz; 0 = off
// Bit 5: 1 = on; 0 = off
// Bit 6: 1 = on; 0 = off
// Bit 7: 1 = on; 0 = off

#define STARTSTATE 0b00000001

UA_DateTimeStruct getCurrentTimestruct() {
    return UA_DateTime_toStruct( UA_DateTime_now());
}

UA_Byte getState(UA_DateTimeStruct dt) {

    //off
    if(dt.hour > 21 || dt.hour < 6)
        return 0b00000000;
    //Moving
    if(dt.sec % 6 == 0)
        return 0b00000111;
    //Nieten
    if(dt.sec % 6 == 1 && dt.milliSec < 500)
        return 0b00000101;
    //Moving + switching
    if(dt.sec % 6 < 3)
        return 0b00010111;
    //Bohren
    if(dt.sec % 6 == 3 && dt.milliSec < 500)
        return 0b00000101;
    //switching to schrauber
    if(dt.sec % 6 < 4)
        return 0b00010101;
    //schreuben
    if(dt.sec % 6 == 4 && dt.milliSec < 500)
        return 0b00000101;
    //moving back + switching
    if(dt.sec % 6 < 5)
        return 0b00010111;

    return 0b00000001;
}


static UA_StatusCode readCurrentState(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp, 
                                        const UA_NumericRange *range, UA_DataValue *dataValue) {
    
    UA_DateTimeStruct dt = getCurrentTimestruct();

        
    UA_Byte  val = getState(dt);
    UA_Variant_setScalarCopy(&dataValue->value, &val,  &UA_TYPES[UA_TYPES_BYTE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

void addCurrentStateSourceVariable(UA_Server *server, int nodeNr) {
        UA_VariableAttributes attr;
    UA_VariableAttributes_init(&attr);
    attr.displayName = UA_LOCALIZEDTEXT("en_US", "Current state - data source");

    UA_NodeId currentNodeId = UA_NODEID_STRING(nodeNr , "current-state-datasource");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(nodeNr , "current-state-datasource");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NULL;

    UA_DataSource stateDataSource;
    stateDataSource.handle = NULL;
    stateDataSource.read = readCurrentState;
    stateDataSource.write = NULL;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        stateDataSource, NULL);

}
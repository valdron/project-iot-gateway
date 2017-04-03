#include "open62541.h"
#include "coordinates.h"


#define START_POS {1.0, 1.0, 1.0}


static UA_StatusCode readCurrentPosition(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp, 
                                        const UA_NumericRange *range, UA_DataValue *dataValue) {
    UA_Double pos[3] = START_POS;
    UA_Variant_setArrayCopy(&dataValue->value, pos, 3,  &UA_TYPES[UA_TYPES_DOUBLE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}



void addCurrentPositionSourceVariable(UA_Server *server, int nodeNr) {
    UA_VariableAttributes attr;
    UA_Int32 dim[] = {3};
    attr.arrayDimensions = dim;
    attr.arrayDimensionsSize = 1;
    UA_VariableAttributes_init(&attr);
    attr.displayName = UA_LOCALIZEDTEXT("en_US", "Current position [x,y,z] - data source");

    UA_NodeId currentNodeId = UA_NODEID_STRING(nodeNr , "current-position-datasource");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(nodeNr , "current-position-datasource");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NULL;

    UA_DataSource timeDataSource;
    timeDataSource.handle = NULL;
    timeDataSource.read = readCurrentPosition;
    timeDataSource.write = NULL;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        timeDataSource, NULL);
}



#include <signal.h>
#include <open62541.h>
#include <stdio.h>
#include <time.h>   
#include <stdlib.h>
#include <unistd.h>
#include "state.h"


//Hardcoded für 3 Nodes mit Wertänderung -> ggf Dynamisch machen
static UA_Variant *t_temperatureAttr,*t_pressureAttr, *t_stateAttr;

static UA_NodeId RoborterarmId = {1,UA_NODEIDTYPE_NUMERIC, {1001}};
static double Temperatur = 40.0;
static double Pressure = 1.0;
static double Drehzahl = 1000.0;
static UA_Byte state = STARTSTATE;

static UA_NodeId *temperatureNodeId;
static UA_NodeId *pressureNodeId;
static UA_NodeId *stateNodeId;


//Initialisieren der Variablen
//Hardcoded für 3 Nodes mit Wertänderung -> ggf Dynamisch machen
void initVariables(){
    temperatureNodeId = UA_NodeId_new();
    UA_NodeId_init(temperatureNodeId);


    pressureNodeId = UA_NodeId_new();
    UA_NodeId_init(pressureNodeId);

    stateNodeId = UA_NodeId_new();
    UA_NodeId_init(stateNodeId);

    //NodeIds werden zugewisen (Keine Optimale Lösung!!!)
    *temperatureNodeId = UA_NODEID_NUMERIC(1,118);
    *pressureNodeId = UA_NODEID_NUMERIC(1,119);
    *stateNodeId = UA_NODEID_NUMERIC(1,120); 

    t_temperatureAttr = UA_Variant_new();
    UA_Variant_init(t_temperatureAttr);

    t_pressureAttr = UA_Variant_new();
    UA_Variant_init(t_pressureAttr);

    t_stateAttr = UA_Variant_new();
    UA_Variant_init(t_stateAttr);
}

//Funktion um Zufällige Werte zu generrieren
static UA_Double generateValues(double obere_grenze, double untere_grenze, double value){
    double deltaTemp = ((double) rand()/ RAND_MAX)*(obere_grenze-untere_grenze) + untere_grenze;
    value += deltaTemp;
    return (UA_Double)value;
}

//**************readFunktionen*************//
//Benutze readfunktionen um NodeIds zu Bekommen
//readFunktionen werden jeweils einmal bei start des Servers aufgerufen um Startwerte zu Initialisieren

//Temperatur on call Funktion
static UA_StatusCode readCurrentTemp(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp,
    const UA_NumericRange *range, UA_DataValue *dataValue) {
    
    printf("ReadTemperature triggerd\n");
    //generateTemperature gibt neue Tempertur zurück vom Typ Double
    UA_Variant_setScalarCopy(&dataValue->value, &Temperatur, &UA_TYPES[UA_TYPES_DOUBLE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
} 

//Pressure on call Funktion
static UA_StatusCode readCurrentPressure(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp,
    const UA_NumericRange *range, UA_DataValue *dataValue) {

    printf("Read Pressure triggerd\n");
    //generateTemperature gibt neue Pressure zurück vom Typ Double
    UA_Variant_setScalarCopy(&dataValue->value, &Pressure, &UA_TYPES[UA_TYPES_DOUBLE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}



static UA_StatusCode readCurrentState(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp, 
                                        const UA_NumericRange *range, UA_DataValue *dataValue) {

    printf("Read State triggerd\n");
    UA_DateTimeStruct dt = getCurrentTimestruct();

    UA_Byte  val = getState(dt);
    UA_Variant_setScalarCopy(&dataValue->value, &val,  &UA_TYPES[UA_TYPES_BYTE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

//**************Threads*************//

//State Threadfunktion
void *askForState(void *serverPtr) {
    printf("State Thread started\n");
    while(true){
        printf("State: %d\n", state);
        //generateTemperature gibt neue Tempertur zurück vom Typ Double
        state = getState(getCurrentTimestruct());
        UA_Variant_setScalar(t_stateAttr, &state, &UA_TYPES[UA_TYPES_BYTE]);
        UA_Server_writeValue(serverPtr,*stateNodeId,*t_stateAttr);
        sleep(1);
    }
} 


//Temperatur Threading Funktion
void *calcCurrentTemp(void *serverPtr) {
    printf("Temperatur Thread started\n");
    while(true){
        printf("Temperatur Wert: %f\n", Temperatur);
        //generateTemperature gibt neue Tempertur zurück vom Typ Double
        Temperatur = generateValues(2.0,-2.0,Temperatur);
        UA_Variant_setScalar(t_temperatureAttr, &Temperatur, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Server_writeValue(serverPtr,*temperatureNodeId,*t_temperatureAttr); //<- alternative NodeId Hardcodederstellen mit bsp. UA_NODEID_NUMERIC(1,119)
        sleep(1);
    }
} 

//Pressure Threading Funktion
void *calcCurrentPressure(void *serverPtr){
    printf("Pressure Thread started\n");
    while(true){
        printf("Pressure Wert:  %f\n", Pressure);
        //generateTemperature gibt neue Pressure zurück vom Typ Double
        Pressure = generateValues(0.02,-0.02,Pressure);
        UA_Variant_setScalar(t_pressureAttr , &Pressure, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Server_writeValue(serverPtr,*pressureNodeId,*t_pressureAttr);
        sleep(1);
    }
}



void defineObjectTypes(UA_Server *server) {
    /* Define the object type for "Device" */
    UA_NodeId deviceTypeId; /* get the nodeid assigned by the server */
    UA_ObjectTypeAttributes deviceAttr;
    UA_ObjectTypeAttributes_init(&deviceAttr);
    deviceAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Roborterarm");

    UA_Server_addObjectTypeNode(server, UA_NODEID_NULL,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "Roborterarm"), deviceAttr,
                                NULL, &deviceTypeId);


    UA_VariableAttributes nameAttr;
    UA_VariableAttributes_init(&nameAttr);
    nameAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Modellname");
    UA_String name = UA_STRING("Robo9000");
    UA_Variant_setScalar(&nameAttr.value, &name, &UA_TYPES[UA_TYPES_STRING]);

    UA_Server_addVariableNode(server, UA_NODEID_NULL, deviceTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Modellname"),
                              UA_NODEID_NULL, nameAttr, NULL, NULL);

    
    /* Define the object type for "Pump" */
    UA_ObjectTypeAttributes ptAttr;
    UA_ObjectTypeAttributes_init(&ptAttr);
    ptAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Roborterarm Values");
    UA_Server_addObjectTypeNode(server, RoborterarmId,
                                deviceTypeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "Roborterarm Values"), ptAttr,
                                NULL, NULL);


    
    //******************Variablen/Messdaten für Roboterarm**************//

    //Temperatur Node
    UA_VariableAttributes tempertureAttr;
    UA_VariableAttributes_init(&tempertureAttr);
    UA_Variant_setScalar(&tempertureAttr.value, &Temperatur, &UA_TYPES[UA_TYPES_DOUBLE]);

    tempertureAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Temperature");
    
    //Handling funktionen für Temperatur
    UA_DataSource temperatureDataSource;
    temperatureDataSource.handle = NULL;
    //Readfunktionen werden für Client_v2 nicht benötigt
    //Damit man aber den OPC Server mit UA Expert untersuchen kann braucht man diese
    //-> UA Expert nutzt read. Ansonsten wird das Item nicht angezeigt
    temperatureDataSource.read = readCurrentTemp;
    temperatureDataSource.write = NULL;
    
    
    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, RoborterarmId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Temperature"),
                              UA_NODEID_NULL, tempertureAttr, temperatureDataSource, NULL);

    //Pressure Node                              
    UA_VariableAttributes pressureAttr;
    UA_VariableAttributes_init(&pressureAttr);
    pressureAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Pressure");
    UA_Variant_setScalar(&pressureAttr.value, &Pressure, &UA_TYPES[UA_TYPES_DOUBLE]);

    
    //Handling funktionen für Temperatur
    UA_DataSource pressureDataSource;
    pressureDataSource.handle = NULL;
    //Readfunktionen werden für Client_v2 nicht benötigt
    //Damit man aber den OPC Server mit UA Expert untersuchen kann braucht man diese
    //-> UA Expert nutzt read. Ansonsten wird das Item nicht angezeigt
    pressureDataSource.read = readCurrentPressure;
    pressureDataSource.write = NULL;

    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, RoborterarmId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Pressure"),
                              UA_NODEID_NULL, pressureAttr, pressureDataSource, NULL);

    //State Node                              
    UA_VariableAttributes stateAttr;
    UA_VariableAttributes_init(&stateAttr);
    stateAttr.displayName = UA_LOCALIZEDTEXT("en_US", "State");
    UA_Variant_setScalar(&stateAttr.value, &state, &UA_TYPES[UA_TYPES_BYTE]);

    
    //Handling funktionen für State
    UA_DataSource stateDataSource;
    stateDataSource.handle = NULL;
    //Readfunktionen werden für Client_v2 nicht benötigt
    //Damit man aber den OPC Server mit UA Expert untersuchen kann braucht man diese
    //-> UA Expert nutzt read. Ansonsten wird das Item nicht angezeigt
    stateDataSource.read = readCurrentState;
    stateDataSource.write = NULL;

    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, RoborterarmId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "State"),
                              UA_NODEID_NULL, stateAttr, stateDataSource, NULL);
}



void addRoboterarmObjectInstance(UA_Server *server, char *name) {
    UA_ObjectAttributes oAttr;
    UA_ObjectAttributes_init(&oAttr);
    oAttr.displayName = UA_LOCALIZEDTEXT("en_US", name);
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, name),
                            RoborterarmId, /* this refers to the object type
                                           identifier */
                            oAttr, NULL, NULL);
}


/*UA_Server *s = NULL; /* required to get the server pointer into the constructor
                        function (will change for v0.3) */


//IN DIESER FUNKTION WURDE ETWAS ENTFERNT. --> SIEHE DOKU
void * roboterarmTypeConstructor(const UA_NodeId instance) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Roborterarm created");

    /* Find the NodeId of the status child variable */
    UA_RelativePathElement rpe;
    UA_RelativePathElement_init(&rpe);
    rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    rpe.isInverse = false;
    rpe.includeSubtypes = false;
    rpe.targetName = UA_QUALIFIEDNAME(1, "Status");    

    /* The return pointer of the constructor is attached to the ObjectNode */
    return NULL;
}

void addRoboterarmTypeConstructor(UA_Server *server) {
    UA_ObjectLifecycleManagement olm;
    olm.constructor = roboterarmTypeConstructor;
    olm.destructor = NULL;
    UA_Server_setObjectTypeNode_lifecycleManagement(server, RoborterarmId , olm);
}


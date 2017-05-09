#include <signal.h>
#include "open62541.h"
#include <stdio.h>
#include <time.h>   
#include <stdlib.h>
#include <unistd.h>



UA_Variant *t_temperatureAttr,*t_pressureAttr;

UA_NodeId RoborterarmId = {1,UA_NODEIDTYPE_NUMERIC, {1001}};
static double Temperatur = 40.0;
double Pressure = 1.0;
double Drehzahl = 1000.0;

//Initialisieren der Variablen
void initVariants(){
    t_temperatureAttr = UA_Variant_new();
    UA_Variant_init(t_temperatureAttr);

    t_pressureAttr = UA_Variant_new();
    UA_Variant_init(t_pressureAttr);
}

//Funktion um Zufällige Werte zu generrieren
UA_Double generateValues(double obere_grenze, double untere_grenze, double value){
    double deltaTemp = ((double) rand()/ RAND_MAX)*(obere_grenze-untere_grenze) + untere_grenze;
    value += deltaTemp;
    return (UA_Double)value;
}

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

//Temperatur Threading Funktion
void *calcCurrentTemp(void *serverPtr) {
    printf("Temperatur Thread started\n");
    while(true){
        printf("Temperatur Wert: %f\n", Temperatur);
        //generateTemperature gibt neue Tempertur zurück vom Typ Double
        Temperatur = generateValues(2.0,-2.0,Temperatur);
        UA_Variant_setScalar(t_temperatureAttr, &Temperatur, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Server_writeValue(serverPtr,UA_NODEID_NUMERIC(1,117),*t_temperatureAttr);
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
        UA_Server_writeValue(serverPtr,UA_NODEID_NUMERIC(1,118),*t_pressureAttr);
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

    
    //Variablen/Messdaten für Roboterarm
    UA_VariableAttributes tempertureAttr;
    UA_VariableAttributes_init(&tempertureAttr);
    UA_Variant_setScalar(&tempertureAttr.value, &Temperatur, &UA_TYPES[UA_TYPES_DOUBLE]);

    tempertureAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Temperature");
    
    //Handling funktionen für Temperatur
    UA_DataSource temperatureDataSource;
    temperatureDataSource.handle = NULL;
    temperatureDataSource.read = readCurrentTemp;
    temperatureDataSource.write = NULL;

    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, RoborterarmId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Temperature"),
                              UA_NODEID_NULL, tempertureAttr, temperatureDataSource, NULL);


    UA_VariableAttributes pressureAttr;
    UA_VariableAttributes_init(&pressureAttr);
    pressureAttr.displayName = UA_LOCALIZEDTEXT("en_US", "Pressure");
    UA_Variant_setScalar(&pressureAttr.value, &Pressure, &UA_TYPES[UA_TYPES_DOUBLE]);

    
    //Handling funktionen für Temperatur
    UA_DataSource pressureDataSource;
    pressureDataSource.handle = NULL;
    pressureDataSource.read = readCurrentPressure;
    pressureDataSource.write = NULL;

    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, RoborterarmId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Pressure"),
                              UA_NODEID_NULL, pressureAttr, pressureDataSource, NULL);
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


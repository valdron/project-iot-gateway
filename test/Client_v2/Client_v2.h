
#include <open62541.h>
#include "queue.h"
#include "datenerfasser.h"

//parameter ist vom Typ IG_Datenerfasser
void *start_OPC_Client(void * parameter);

//Typen der Nodes HARDCODED <- ggf zu verbessern
enum itemType{
        ROBOTERARM_TEMPERATURE_DOUBLE_VALUE = 117,
        ROBOTERARM_PRESSURE_DOUBLE_VALUE = 118
};

//To save data of the Items
typedef struct {
    UA_UInt32 subId;
    UA_UInt32 monId;
    enum itemType type;
    IG_Queue * queue; 
    bool avgValue;
}MonitoredItems;



// DEFINES FOR CONSTANTS
#define IG_DATA_EMPTY {0, IG_NULL, NULL, 0}



//TYPEDEFS
// Time in 100ns intervals since 01.01.1601;
typedef int64_t IG_DateTime;

// Internal Identification number
typedef uint32_t IG_ID; 

typedef bool IG_BOOL;
typedef uint32_t IG_UInt32;
typedef uint64_t IG_UInt64;
typedef int32_t IG_Int32;
typedef int64_t IG_Int64;
typedef float IG_Float;
typedef double IG_Double;
typedef int32_t IG_Datatype;
//TODO: ... noch mehr?

// IG Statuscode
// TODO: 
enum IG_Status {
    IG_STATUS_GOOD, 
    IG_STATUS_BAD,
    IG_STATUS_QUEUE_FULL
};


//TODO: insert relevant Datatypes (floatingpoint, integer, )
enum IG_Datatype {
    IG_DOUBLE, 
    IG_NULL
    /*...*/
};


// IG Data
typedef struct {
    IG_ID id;
    IG_Datatype datatype;
    void * data;
    IG_DateTime timestamp;
    bool avgValue;
}IG_Data;

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
    bool avgValue;
}MonitoredItems;
// DEFINES FOR CONSTANTS
#define IG_DATA_EMPTY {0, IG_NULL, NULL, 0}
// Just for purpose
#define enum bool {false,true}


//TYPEDEFS
// Time in 100ns intervals since 01.01.1601;
typedef int64_t IG_DateTime;

// Internal Identification number
typedef uint32_t IG_ID; 

typedef bool IG_Bool;
typedef uint32_t IG_UInt32;
typedef uint64_t IG_UInt64;
typedef int32_t IG_Int32;
typedef int64_t IG_Int64;
typedef float IG_Float;
typedef double IG_Double;
//TODO: ... noch mehr?

// IG Statuscode
// TODO: 
typedef enum IG_Status {
	IG_STATUS_GOOD,
	IG_STATUS_BA,
	IG_STATUS_QUEUE_FULL,
} IG_Status;

//TODO: insert relevant Datatypes (floatingpoint, integer, )
typedef enum IG_Datatype {
    IG_DOUBLE, 
    IG_NULL,
	IG_INT,
	IG_CHAR,
	IG_BOOL
    /*...*/
} IG_Datatype;


// IG Data

typedef struct IG_Data{
	IG_ID id;
	IG_Datatype datatype;
	void * data;
	IG_DateTime timestamp;
} IG_Data;


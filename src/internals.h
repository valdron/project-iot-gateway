
//TODO: insert relevant Datatypes (floatingpoint, integer, )
typedef enum IG_Datatype {IG_DOUBLE/*...*/} IG_Datatype;


// Time in 100ns intervals since 01.01.1601;
typedef int64_t IG_DateTime;

typedef uint32_t IG_ID; 


typedef struct IG_Data{
    IG_ID id,
    IG_Datatype datatype,
    void * data,
    IG_DateTime timestamp
} IG_Data;

typedef enum IG_Status {IG_STATUS_GOOD, IG_STATUS_BAD} IG_Status;
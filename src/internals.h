
//TODO: insert relevant Datatypes (floatingpoint, integer, )
typedef enum IG_Datatype {IG_DOUBLE/*...*/} IG_Datatype;

typedef struct IG_Data{
    IG_Datatype datatype,
    * void data
} IG_Data;

typedef enum IG_Status {IG_STATUS_GOOD, IG_STATUS_BAD} IG_Status;
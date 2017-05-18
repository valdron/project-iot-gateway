#ifndef INTERNALS_H
#define INTERNALS_H

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>

// DEFINES FOR CONSTANTS


//TYPEDEFS
// Time in 100ns intervals since 01.01.1601;
typedef int64_t IG_DateTime;

// Internal Identification number
typedef uint32_t IG_Id; 

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
typedef enum {
    IG_STATUS_GOOD, 
    IG_STATUS_BAD,
    IG_STATUS_QUEUE_FULL
    /*add more as needed*/
} IG_Status;

//TODO: insert relevant Datatypes (floatingpoint, integer, )
typedef enum {
    IG_DOUBLE, 
    IG_FLOAT,
    IG_NULL,
    IG_INT32,
    IG_UINT32,
    IG_INT64,
    IG_UINT64,
    IG_DATETIME,
    IG_BOOL,
    /*...*/
} IG_Datatype;


// IG Data
typedef struct{
	IG_Id id;
	IG_Datatype datatype;
	void * data;
	IG_DateTime timestamp;
} IG_Data;


static const IG_Data IG_DATA_EMPTY = (IG_Data){0, IG_NULL, NULL, 0};

// Creates new IG_Data on the heap and inits it with the parameters
IG_Data * IG_Data_create(IG_Id id, IG_Datatype type, void * data, IG_DateTime time);

//care this doesnt free the memory needed for the data pointer
void IG_Data_delete(IG_Data * data);

// this frees the data ptr inside
void IG_Data_delete_members(IG_Data * data);
#endif

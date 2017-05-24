
#ifndef INTERNALS_H
#define INTERNALS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//TYPEDEFS AND CONSTANTS


// Internal Identification number
typedef uint32_t IG_Id; 

typedef bool IG_Bool;
typedef uint32_t IG_UInt32;
typedef uint64_t IG_UInt64;
typedef int32_t IG_Int32;
typedef int64_t IG_Int64;
typedef uint8_t IG_Byte;
typedef float IG_Float;
typedef double IG_Double;
typedef uint8_t IG_Byte;
//TODO: ... noch mehr?


typedef unsigned char IG_Char;

//Duration in 100ns intervals
typedef int64_t IG_Duration;

static const IG_Duration IG_DURATION_MICROS = 10;
static const IG_Duration IG_DURATION_MILLIS = 10000;
static const IG_Duration IG_DURATION_SECS   = 10000000;

// Time in 100ns intervals since 01.01.1601;
typedef int64_t IG_DateTime;
static const IG_DateTime IG_DATETIME_UNIX_TIME = 116444736000000000;

//returns the current time
IG_DateTime IG_DateTime_now();

//returns the new time for @time + @duration 
IG_DateTime IG_DateTime_add_duration(IG_DateTime time, IG_Duration duration);

//returns Duration from a milliseconds amount
IG_Duration IG_Duration_from_ms(IG_UInt64 ms);

//returns Duration from a seconds amount
IG_Duration IG_Duration_from_sec(IG_UInt64 s);



//contains string length in bytes and the string not null terminated
typedef struct {
    IG_UInt64 len;
    IG_Byte * data;
} IG_String;

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
    IG_BYTE,
    IG_DOUBLE, 
    IG_FLOAT,
    IG_NULL,
    IG_INT32,
    IG_UINT32,
    IG_INT64,
    IG_UINT64,
    IG_DATETIME,
    IG_DURATION,
    IG_BOOL
    /*...*/
} IG_Datatype;


// IG Data
typedef struct{
	IG_Id id;
	IG_Datatype datatype;
    IG_DateTime timestamp;
	void * data;
} IG_Data;

// Creates new IG_Data on the heap and inits it with the parameters
IG_Data * IG_Data_create(IG_Id id, IG_Datatype type, void * data, IG_DateTime time);

// This function is creating an uninitialized IG_Data struct
IG_Data * IG_Data_create_empty();

//care this doesnt free the memory needed for the data pointer
void IG_Data_delete(IG_Data * data);

// this frees the data ptr inside
void IG_Data_delete_members(IG_Data * data);

#endif

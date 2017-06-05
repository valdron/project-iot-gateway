
#include "internals.h"
#include<time.h>
#include<string.h>

IG_Data * IG_Data_create(IG_Id id, IG_Datatype type, void * dataptr, IG_DateTime timestamp) {
    IG_Data * data = (IG_Data *) malloc(sizeof(IG_Data));
    data->id = id;
    data->datatype = type;
    data->data = dataptr;
    data->timestamp = timestamp;
    return data;
}

//care this doesnt free the memory needed for the data pointer
void IG_Data_delete(IG_Data * data){
    free(data);
}

// this frees the data ptr inside
void IG_Data_delete_members(IG_Data * data){
    free(data->data);
}

IG_DateTime IG_DateTime_now() {
    time_t t = time(NULL);
    IG_DateTime dt = t * IG_DURATION_SECS + IG_DATETIME_UNIX_TIME;
    return dt;
}

IG_DateTime IG_DateTime_add_duration(IG_DateTime time, IG_Duration duration) {
    return (time + duration);
}

IG_Duration IG_Duration_from_ms(IG_UInt64 ms) {
    IG_Duration dt = ms * IG_DURATION_MILLIS;
    return dt;
}

IG_Duration IG_Duration_from_sec(IG_UInt64 s) {
    IG_Duration dt = s * IG_DURATION_SECS;
    return dt;
}


// This function is creating an uninitialized IG_Data struct
IG_Data *IG_Data_create_empty(){
    return (IG_Data *) malloc(sizeof(IG_Data));
}

IG_Datatype IG_Datatype_from_str(IG_Char * str) {
    if(!strcmp(str, IG_BYTE_TYPENAME)) {
        return IG_BYTE;
    } else if(!strcmp(str, IG_DOUBLE_TYPENAME)) {
        return IG_DOUBLE;
    } else if(!strcmp(str, IG_FLOAT_TYPENAME)) {
        return IG_FLOAT;
    } else if(!strcmp(str, IG_INT32_TYPENAME)) {
        return IG_INT32;
    } else if(!strcmp(str, IG_UINT32_TYPENAME)) {
        return IG_UINT32;
    } else if(!strcmp(str, IG_INT64_TYPENAME)) {
        return IG_INT64;
    } else if(!strcmp(str, IG_UINT64_TYPENAME)) {
        return IG_UINT64;
    } else if(!strcmp(str, IG_DATETIME_TYPENAME)) {
        return IG_DATETIME;
    } else if(!strcmp(str, IG_DURATION_TYPENAME)) {
        return IG_DURATION;
    } else if(!strcmp(str, IG_BOOL_TYPENAME)) {
        return IG_BOOL;
    } else {
        return IG_NULL;
    }
}


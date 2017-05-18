#include "internals.h"
#include<time.h>

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



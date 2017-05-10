#include "internals.h"

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
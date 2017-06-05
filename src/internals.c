
#include "internals.h"
#include<time.h>
#include<string.h>

#define BUFFSIZE 30


IG_Data * IG_Data_create(IG_Id id, IG_Datatype type, void * dataptr, IG_DateTime timestamp) {
    IG_Data * data = (IG_Data *) malloc(sizeof(IG_Data));
    data->id = id;
    data->datatype = type;
    data->data = dataptr;
    data->timestamp = timestamp;
    return data;
}

void * IG_Data_allocate(IG_Datatype type) {
    switch(type) {
    case IG_BYTE:
        return malloc(sizeof(IG_Byte));
    break;

    case IG_DOUBLE:
        return malloc(sizeof(IG_Double));
    break;

    case IG_FLOAT:
        return malloc(sizeof(IG_Float));
    break;

    case IG_NULL:
        return NULL;
    break;

    case IG_INT32:
        return malloc(sizeof(IG_Int32));
    break;

    case IG_UINT32:
        return malloc(sizeof(IG_UInt32));
    break;

    case IG_INT64:
        return malloc(sizeof(IG_Int64));
    break;

    case IG_UINT64:
        return malloc(sizeof(IG_UInt64));
    break;

    case IG_DATETIME:
        return malloc(sizeof(IG_DateTime));
    break;

    case IG_DURATION:
        return malloc(sizeof(IG_Duration));
    break;

    case IG_BOOL:
        return malloc(sizeof(IG_Bool));
    break;

    case IG_CHAR:
        return malloc(sizeof(IG_Char));
    break;

    default:
        return NULL;
    }
}


//care this doesnt free the memory needed for the data pointer
void IG_Data_delete(IG_Data * data){
    free(data);
}

// this frees the data ptr inside
void IG_Data_delete_members(IG_Data * data){
    free(data->data);
}

IG_Char * IG_Data_toString(IG_Data* data){
	IG_Char* value = malloc(BUFFSIZE);
	void* dataToEncode = data->data;
	switch(data->datatype){
		case IG_DOUBLE:
		case IG_FLOAT:
			snprintf(value, BUFFSIZE, "%f", *((IG_Float*)dataToEncode));
			break;
		case IG_INT32:
			snprintf(value, BUFFSIZE, "%i", *((IG_Int32*)dataToEncode));
			break;
		case IG_UINT32:
			snprintf(value, BUFFSIZE, "%u", *((IG_UInt32*)dataToEncode));
			break;
		case IG_INT64:
			snprintf(value, BUFFSIZE, "%ld",*((IG_Int64*)dataToEncode));
			break;
		case IG_UINT64:
		case IG_DATETIME:
		case IG_DURATION:
			snprintf(value, BUFFSIZE, "%lu",*((IG_UInt64*)dataToEncode));
			break;
		case IG_BYTE:
			snprintf(value, BUFFSIZE, "%hu",*((IG_Byte*)dataToEncode));
			break;
		case IG_BOOL:
			snprintf(value, BUFFSIZE, "%s", *((IG_Bool*)dataToEncode)?"TRUE":"FALSE");
			break;
		case IG_CHAR:
			snprintf(value, BUFFSIZE, "%hhu",*((IG_Char*)dataToEncode));
			break;
		case IG_NULL:
			snprintf(value, BUFFSIZE, "NULL");
			break;
		default:
			snprintf(value, BUFFSIZE, "ERROR");
			break;
	}
	return value;
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


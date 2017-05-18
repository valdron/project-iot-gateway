#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include "internals.h"
#include <open62541.h>

void handler_TheAnswerChanged(UA_UInt32 monId, UA_DataValue *value, void *context);

#endif
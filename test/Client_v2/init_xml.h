
#ifndef INIT_XML_H
#define INIT_XML_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "queue.h"
#include "Client_v2.h"

int init(UA_Client *client, MonitoredItems *monitoredItems, IG_Queue *queue);

#endif
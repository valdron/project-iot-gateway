#include "queue.h"
#include "datenerfasser.h"

IG_Datenerfasser * IG_Datenerfasser_create_nonBlocking(IG_Config * config) {
    IG_Datenerfasser * erfasser = (IG_Datenerfasser *) malloc(sizeof(IG_Datenerfasser));
    erfasser->config = config;
    erfasser->queue = IG_Queue_new(IG_QUEUE_NONBLOCKING);
    return erfasser;
}


void IG_Datenerfasser_delete(IG_Datenerfasser * erfasser) {
    IG_Queue_delete(erfasser->queue);
    free(erfasser);
}

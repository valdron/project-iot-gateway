#include "datenversender.h"

IG_Datenversender * IG_Datenversender_create(IG_Config * config) {
    IG_Datenversender * versender = (IG_Datenversender *) malloc(sizeof(IG_Datenversender));
    versender->config = config;
    versender->queue = IG_Queue_new();
    return versender;
}


void IG_Datenversender_delete(IG_Datenversender * versender) {
    IG_Queue_delete(versender->queue);
    free(versender);
}
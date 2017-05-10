#include "verarbeiter.h"


IG_Verarbeiter * IG_Verarbeiter_create(IG_Config * config, IG_Datenversender * sender, IG_Datenerfasser * erfasser){
    //create new on the heap
    IG_Verarbeiter * arbeiter = (IG_Verarbeiter *) malloc(sizeof(IG_Verarbeiter));
    arbeiter->config = config;
    arbeiter->sender = sender;
    arbeiter->erfasser = erfasser;
    return arbeiter;
}


void IG_Verarbeiter_delete(IG_Verarbeiter * verarbeiter) {
    free(verarbeiter);
}

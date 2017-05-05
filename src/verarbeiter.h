#ifndef VERARBEITER_H
#define VERARBEITER_H

// this file shall contain the main working loop
#include "internals.h"
#include "datenversender.h"
#include "datenerfasser.h"

typedef struct{
    IG_Config * config;
    IG_Datenversender * sender;
    IG_Datenerfasser * erfasser;
} IG_Verarbeiter;

//initializes the struct and starts the thread-loop
IG_Status init_verarbeiter(IG_Verarbeiter * verarbeiter);

//creates new struct on the heap and initializes it with the parameters
IG_Verarbeiter * IG_Verarbeiter_create(IG_Config * config, IG_Datenversender * sender, IG_Datenerfasser * erfasser);

// frees the memory needed for the struct
void IG_Verarbeiter_delete(IG_Verarbeiter * verarbeiter);

#endif
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
    IG_Bool running;
} IG_Verarbeiter;

typedef enum{
	IG_RULE_TRANSMIT,
	IG_RULE_AVG,
	IG_RULE_MAX,
	IG_RULE_MIN
} IG_RuleType;

typedef struct{
	IG_RuleType rule;
	IG_INT32 outputId;
	IG_DateTime deadline;
	IG_DateTime interval;
	IG_Data* data;
} IG_Rule;

typedef struct{
	IG_INT32 inputId;
	IG_Datatype datatype;
	IG_INT32 ruleSize; //number of rules in this RuleSet
	IG_Rule* rules;	// pointer to an array of rules	
} IG_Input_RuleSet;

typedef struct{
	IG_Verarbeiter* verarbeiter;
	IG_INT32 ruleSetSize;
	IG_Input_RuleSet* ruleSets;
} workLoopArgs;


//initializes the struct and starts the thread-loop
IG_Status init_verarbeiter(IG_Verarbeiter * verarbeiter);

//creates new struct on the heap and initializes it with the parameters
IG_Verarbeiter * IG_Verarbeiter_create(IG_Config * config, IG_Datenversender * sender, IG_Datenerfasser * erfasser);

// frees the memory needed for the struct
void IG_Verarbeiter_delete(IG_Verarbeiter * verarbeiter);

void* workLoop(void*);

#endif

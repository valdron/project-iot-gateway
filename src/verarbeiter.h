#ifndef VERARBEITER_H
#define VERARBEITER_H

// this file shall contain the main working loop
#include "internals.h"
#include "datenversender.h"
#include "datenerfasser.h"

// Function pointer prototype
void (*IG_funcPtr)(IG_Data *,IG_Rule *);

void IG_avg(IG_Data *,IG_Data *);
void IG_max(IG_Data *,IG_Data *);
void IG_min(IG_Data *,IG_Data *);
void IG_transmit(IG_Data *,IG_Data *);

// Struct for the Verarbeiter
typedef struct{
    IG_Config * config;
    IG_Datenversender * sender;
    IG_Datenerfasser * erfasser;
    IG_Bool running;
} IG_Verarbeiter;


// Enumeration for rule types
typedef enum{
	IG_RULE_TRANSMIT,
	IG_RULE_AVG,
	IG_RULE_MAX,
	IG_RULE_MIN
} IG_RuleType;

// Struct for an rule
typedef struct{
	IG_RuleType rule;
	IG_Int32 outputId;
	IG_funcPtr function;
	IG_DateTime deadline;
	IG_DateTime interval;
	IG_Int32 size;	// Number of Data
	IG_Data * data;
} IG_Rule;

// Struct for an set of rules
typedef struct{
	IG_Int32 inputId;
	IG_Datatype datatype;
	IG_Int32 ruleSize; //number of rules in this RuleSet
	IG_Rule * rules;	// pointer to an array of rules	
} IG_Input_RuleSet;

// Struct to pass agurments to the workLoop
typedef struct{
	IG_Verarbeiter * verarbeiter;
	IG_Int32 ruleSetSize;
	IG_Input_RuleSet* ruleSetArray;
} IG_WorkLoopArgs;


// Initializes the struct and starts the thread-loop
IG_Status IG_verarbeiter_init(IG_Verarbeiter * verarbeiter);

// Creates new struct on the heap and initializes it with the parameters
IG_Verarbeiter * IG_Verarbeiter_create(IG_Config * config, IG_Datenversender * sender, IG_Datenerfasser * erfasser);

// Frees the memory needed for the struct
void IG_Verarbeiter_delete(IG_Verarbeiter * verarbeiter);

// Function that apply rules on data
void IG_Verarbeiter_applyRules(IG_Data * data,IG_Input_RuleSet * ruleSetArray, IG_Int32 ruleSetSize);
void IG_Verarbeiter_applyRule(IG_Data * data,IG_Input_RuleSet * ruleSet);

// Function to check if data has to be send
void IG_Verarbeiter_checkIntervals(IG_Input_RuleSet * ruleSet, IG_Int32 ruleSetSize, IG_Queue * queue);

// Encodes data into JSON format
IG_Data * encodeToJSON(IG_Rule *);

// Function that proccesses all the data
void* IG_WorkLoop(void * args);

#endif

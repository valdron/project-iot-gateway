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

IG_Status init_verarbeiter(IG_Verarbeiter * verarbeiter){
	//Insert magic here for config where rules sets are created
	
	//Output of magic is an array of IG_Input_RuleSet	
	IG_INT32 size= 0;
	IG_Input_RuleSet ruleSets[size]= NULL;
	
	// TODO: Malloc so its on the heap
	IG_WorkLoopArgs args = (workLoopArgs){verarbeiter, ruleSets, size};
	pthread_t thread;	

	pthread_create(&thread,NULL,IG_WorkLoop,(void*)&args)

	return IG_GOOD;
}

void* IG_WorkLoop(void * argument){
	// Parse argmuents
	IG_WorkLoopArgs args = (IG_WorkLoopArgs *)argument;
	Verarbeiter * verarbeiter = args->verarbeiter;
	IG_Int32 ruleSetSize = args->ruleSetSize;
	// Array of RuleSets
	IG_Input_RuleSet * ruleSetArray = args->ruleSetArray:
	
	IG_Queue * queueErfasser = verarbeiter->erfasser->queue;
	IG_Queue * queueSender = verarbeiter->sender->queue;

	// endless loop
	while(1){
		/*
		Maybe add some special variable which terminates loop directly (For error cases)
		if(fatalError) break;
		*/
		if(!verarbeiter->running && IG_Queue_isEmpty(queueErfasser))
			break;
		
		IG_Data* data = IG_Queue_take(queueErfasser);

		// If there is data apply all rules
		if(data!=NULL)
			IG_Verarbeiter_applyRules(data,ruleSetArray, ruleSetSize);

		IG_Verarbeiter_checkIntervals(ruleSetArray, ruleSetSize, queueSender);
		// TODO: Maybe add some functions to create logs/print status
	}
}

void IG_Verarbeiter_applyRules(IG_Data * data,IG_Input_RuleSet * ruleSetArray, IG_Int32 ruleSetSize){
	// Find the correct RuleSet
	for(IG_Int32 i = 0; i < ruleSetSize; i++){
		if(ruleSetArray[i].inputId = data->id){
			// Apply entire RuleSet on data
			IG_Verarbeiter_applyRule(data, ruleSetArray[i]);
			break;
		}				
	}

}

void IG_Verarbeiter_applyRule(IG_Data * data, IG_Input_RuleSet* ruleSet){
	// Call all rule functions and invoke the data and the rule specific data
	for(IG_Int32 i = 0; i < (ruleSet->ruleSize);i++){
		(*(ruleSet->rules[i].function))(data, ruleSet->rules[i].data);
	}
}

void IG_Verarbeiter_checkIntervals(IG_Input_RuleSet * ruleSetArray, IG_Int32 ruleSetSize, IG_Queue * queue){
	// Go through every rule and check if to Send
	IG_DateTime now = IG_DateTime_now();
	for(IG_Int32 i = 0; i < ruleSetSize; i++){
		IG_Input_RuleSet * ruleSet = &(ruleSetArray[i]);
		for(IG_Int32 i = 0; i < ruleSet->ruleSize; i++){
			if(ruleSet->rules[i].deadline < now){
				IG_Rule * rule = &(ruleSet->rules[i]);
				IG_Data * dataToSend = encodeToJSON(rule);
				IG_Queue_put(queue);
				rule->deadline = IG_DateTime_add(rule->deadline,rule->interval);
			}	
		}							
	}
}

// TODO:
IG_Data * encodeToJSON(IG_Rule * rule){
	return NULL
}


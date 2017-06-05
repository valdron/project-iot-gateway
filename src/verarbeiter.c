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
    //TODO deletes vom sender und erfasser aufrufen evtl.
    free(verarbeiter);
}

IG_Status IG_Verarbeiter_init(IG_Verarbeiter * verarbeiter){

	IG_ConfigResponse res;
	IG_Status rt = IG_Config_Verarbeiter_get_RuleSets(verarbeiter->config,&res);
	if(rt != IG_STATUS_GOOD) {
		return rt;
       	}

     	IG_WorkLoopArgs* args = (IG_WorkLoopArgs*)malloc(sizeof(IG_WorkLoopArgs));

	args->verarbeiter = verarbeiter;
	args->ruleSetSize = res.responseAmount;
	args->ruleSetArray = (IG_Input_RuleSet *) res.data;

	for(IG_UInt32 i = 0; i < args->ruleSetSize; i++) {
        	IG_ConfigResponse res_rules;
        	rt = IG_Config_Verarbeiter_get_Rules(verarbeiter->config, &res_rules, args->ruleSetArray[i].inputId);
        	if(rt != IG_STATUS_GOOD) {
			return rt;
       		}

        	args->ruleSetArray[i].ruleSize = res_rules.responseAmount;
        	args->ruleSetArray[i].rules = (IG_Rule *) res_rules.data;
    	}

	//Assign function pointers
	IG_Verarbeiter_initFunktionen(args->ruleSetArray, args->ruleSetSize);

	verarbeiter->running = true;

	pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t));
	
	pthread_create(thread,NULL,IG_WorkLoop,(void*)args);

	return IG_STATUS_GOOD;
}

void* IG_WorkLoop(void * argument){

	// Parse argmuents
	IG_WorkLoopArgs* args = (IG_WorkLoopArgs *)argument;
	IG_Verarbeiter * verarbeiter = args->verarbeiter;
	IG_UInt32 ruleSetSize = args->ruleSetSize;

	// Array of RuleSets
	IG_Input_RuleSet * ruleSetArray = args->ruleSetArray;

	IG_Queue * queueErfasser = verarbeiter->erfasser->queue;
	IG_Queue * queueSender = verarbeiter->sender->queue;

	// endless loop
	while(1){
		printf("Workloop working\n");
		/*
		Maybe add some special variable which terminates loop directly (For error cases)
		if(fatalError) break;
		*/
		if(!verarbeiter->running && IG_Queue_isEmpty(queueErfasser))
			break;
		IG_Data* data = IG_Queue_take(queueErfasser);
		printf("Taking Data at %p from Queue\n", data);
		// If there is data apply all rules
		if(data!=NULL){
			printf("Trying to apply rules\n");
			IG_Verarbeiter_applyRules(data, ruleSetArray, ruleSetSize);
			data=NULL;
		}

		IG_Verarbeiter_checkIntervals(ruleSetArray, ruleSetSize, queueSender);
		// TODO: Maybe add some functions to create logs/print status
	}
}

void IG_Verarbeiter_applyRule(IG_Data * data, IG_Input_RuleSet* ruleSet){
	printf("Applying specific rules");
	// Call all rule functions and invoke the data and the rule specific data
	for(IG_UInt32 i = 0; i < (ruleSet->ruleSize);i++){
		(*(ruleSet->rules[i].function))(data, &(ruleSet->rules[i]));
	}
}

void IG_Verarbeiter_applyRules(IG_Data * data, IG_Input_RuleSet * ruleSetArray, IG_UInt32 ruleSetSize){
	printf("Applying all rules\n");
	// Find the correct RuleSet
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		if(ruleSetArray[i].inputId = data->id){
			// Apply entire RuleSet on data
			printf("Found ruleSet for id %d\n", data->id);
			printf("%p\n", &IG_Verarbeiter_applyRule);
			IG_Verarbeiter_applyRule(data, &(ruleSetArray[i]));
			break;
		}				
	}

}


void IG_Verarbeiter_checkIntervals(IG_Input_RuleSet * ruleSetArray, IG_UInt32 ruleSetSize, IG_Queue * queue){
	printf("Checking intervals");
	// Go through every rule and check if to Send
	IG_DateTime now = IG_DateTime_now();
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		IG_Input_RuleSet * ruleSet = &(ruleSetArray[i]);
		for(IG_UInt32 j = 0; j < ruleSet->ruleSize; j++){
			if(ruleSet->rules[j].deadline < now){
				IG_Rule rule = ruleSet->rules[j];
				IG_Data * dataToSend = malloc(sizeof(IG_Data));
				dataToSend->id = rule.outputId;
				dataToSend->datatype = IG_CHAR;
				dataToSend->data = (void*)IG_Verarbeiter_encodeToJSON(rule.data);
				dataToSend->timestamp = IG_DateTime_now();
				IG_Queue_put(queue, dataToSend);
				rule.deadline = IG_DateTime_add_duration(rule.deadline,rule.interval);
			}	
		}							
	}
}

void IG_Verarbeiter_initFunktionen(IG_Input_RuleSet* ruleSetArray, IG_UInt32 ruleSetSize){
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		IG_Input_RuleSet* ruleSet = &(ruleSetArray[i]);
		printf("%p with items: %u\n", ruleSet, ruleSet->ruleSize);
		for(IG_UInt32 j = 0; j < ruleSet->ruleSize; j++){
			IG_Rule* rule = &(ruleSet->rules[j]);
			printf("%p\n", rule);
			switch(rule->rule){
				case IG_RULE_TRANSMIT:
					rule->function = IG_Transmit;
					break;
				case IG_RULE_AVG:
					rule->function = IG_Average;
					break;
				case IG_RULE_MAX:
					rule->function = IG_Maximum;
					break;
				case IG_RULE_MIN:
					rule->function = IG_Minimum;
					break;
				default:
					printf("ERROR\n");
					break;
			}			
		}
	}
}

IG_Char* IG_Verarbeiter_encodeToJSON(IG_Data* data){
	char* s;
	strcat(s,"{value:");
	strcat(s,(char*)IG_Data_toString(data));
	strcat(s,"}");
	return (IG_Char*)s;
}

void IG_Transmit(IG_Data* data, IG_Rule * rule){
	rule->data = data;
}
void IG_Average(IG_Data* data, IG_Rule * rule){
	switch(data->datatype){
		case IG_FLOAT:
			*((IG_Float*)rule->data->data) += *((IG_Float*)data->data);
			rule->size++;
			break;
		case IG_DOUBLE:
			*((IG_Double*)rule->data->data) += *((IG_Double*)data->data);			
			rule->size++;
			break;
		case IG_INT32:
			*((IG_Int32*)rule->data->data) += *((IG_Int32*)data->data);			
			rule->size++;
			break;
		case IG_UINT32:
			*((IG_UInt32*)rule->data->data) += *((IG_UInt32*)data->data);			
			rule->size++;
			break;
		case IG_INT64:
			*((IG_Int64*)rule->data->data) += *((IG_Int64*)data->data);			
			rule->size++;
			break;
		case IG_UINT64:
		case IG_DURATION:
		case IG_DATETIME:
			*((IG_UInt64*)rule->data->data) += *((IG_UInt64*)data->data);			
			rule->size++;
			break;
		case IG_BYTE:
			*((IG_Byte*)rule->data->data) += *((IG_Byte*)data->data);			
			rule->size++;
			break;
		case IG_CHAR:
			*((IG_Char*)rule->data->data) += *((IG_Char*)data->data);			
			rule->size++;
			break;
		case IG_BOOL:
			*((IG_Bool*)rule->data->data) += *((IG_Bool*)data->data);			
			rule->size++;
			break;
		default:
			break;
	}
}
void IG_Maximum(IG_Data* data, IG_Rule * rule){
	switch(data->datatype){
		case IG_FLOAT:	
			if(*((IG_Float*)rule->data->data) > *((IG_Float*)data->data)) return;
			rule->data = data;
			break;
		case IG_DOUBLE:		
			if(*((IG_Double*)rule->data->data) > *((IG_Double*)data->data)) return;
			rule->data = data;
			break;
		case IG_INT32:		
			if(*((IG_Int32*)rule->data->data) > *((IG_Int32*)data->data)) return;
			rule->data = data;
			break;
		case IG_UINT32:	
			if(*((IG_UInt32*)rule->data->data) > *((IG_UInt32*)data->data)) return;
			rule->data = data;
			break;
		case IG_INT64:					
			if(*((IG_Int64*)rule->data->data) > *((IG_Int64*)data->data)) return;
			rule->data = data;
			break;	
		case IG_UINT64:
		case IG_DURATION:
		case IG_DATETIME:	
			if(*((IG_UInt64*)rule->data->data) > *((IG_UInt64*)data->data)) return;
			rule->data = data;
			break;	
		case IG_BYTE:		
			if(*((IG_Byte*)rule->data->data) > *((IG_Byte*)data->data)) return;
			rule->data = data;
			break;
		case IG_CHAR:		
			if(*((IG_Char*)rule->data->data) > *((IG_Char*)data->data)) return;
			rule->data = data;
			break;
		case IG_BOOL:		
			if(*((IG_Bool*)rule->data->data) > *((IG_Bool*)data->data)) return;
			rule->data = data;
		default:
			break;
	}
}
void IG_Minimum(IG_Data* data, IG_Rule * rule){
	switch(data->datatype){
		case IG_FLOAT:	
			if(*((IG_Float*)rule->data->data) < *((IG_Float*)data->data)) return;
			rule->data = data;
			break;
		case IG_DOUBLE:		
			if(*((IG_Double*)rule->data->data) < *((IG_Double*)data->data)) return;
			rule->data = data;
			break;
		case IG_INT32:		
			if(*((IG_Int32*)rule->data->data) < *((IG_Int32*)data->data)) return;
			rule->data = data;
			break;
		case IG_UINT32:	
			if(*((IG_UInt32*)rule->data->data) < *((IG_UInt32*)data->data)) return;
			rule->data = data;
			break;
		case IG_INT64:					
			if(*((IG_Int64*)rule->data->data) < *((IG_Int64*)data->data)) return;
			rule->data = data;
			break;	
		case IG_UINT64:
		case IG_DURATION:
		case IG_DATETIME:	
			if(*((IG_UInt64*)rule->data->data) < *((IG_UInt64*)data->data)) return;
			rule->data = data;
			break;	
		case IG_BYTE:		
			if(*((IG_Byte*)rule->data->data) < *((IG_Byte*)data->data)) return;
			rule->data = data;
			break;
		case IG_CHAR:		
			if(*((IG_Char*)rule->data->data) < *((IG_Char*)data->data)) return;
			rule->data = data;
			break;
		case IG_BOOL:		
			if(*((IG_Bool*)rule->data->data) < *((IG_Bool*)data->data)) return;
			rule->data = data;
		default:
			break;
	}
}

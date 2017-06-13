#include "verarbeiter.h"
#include<unistd.h>


IG_Verarbeiter * IG_Verarbeiter_create(IG_Config * config, IG_Datenversender * sender, IG_Datenerfasser * erfasser){
    //create new on the heap
    IG_Verarbeiter * arbeiter = (IG_Verarbeiter *) malloc(sizeof(IG_Verarbeiter));
    arbeiter->config = config;
    arbeiter->sender = sender;
    arbeiter->erfasser = erfasser;
	arbeiter->running = true;
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

void IG_Verarbeiter_stop(IG_Verarbeiter * verarbeiter) {
	verarbeiter->running = false;
	pthread_join(verarbeiter->th_loop, NULL);
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
	
	printf("Verarbeiter Thread started!\n");


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
		if(data!=NULL){
			IG_Verarbeiter_applyRules(data, ruleSetArray, ruleSetSize);
			IG_Data_delete_members(data);
			IG_Data_delete(data);
		}


		IG_Verarbeiter_checkIntervals(ruleSetArray, ruleSetSize, queueSender);
		// TODO: Maybe add some functions to create logs/print status
	}
	printf("shutting down verarbeiter!\n");
}

void IG_Verarbeiter_applyRule(IG_Data * data, IG_Input_RuleSet* ruleSet){


	// Call all rule functions and invoke the data and the rule specific data
	for(IG_UInt32 i = 0; i < (ruleSet->ruleSize);i++){
		(&ruleSet->rules[i])->function(data, &ruleSet->rules[i]);
	}
}

void IG_Verarbeiter_applyRules(IG_Data * data, IG_Input_RuleSet * ruleSetArray, IG_UInt32 ruleSetSize){
	// Find the correct RuleSet
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		if(ruleSetArray[i].inputId == data->id){	

			// Checking if data is correct data type
			if(data->datatype != ruleSetArray[i].datatype){
				printf("Did not apply Rule\nIncorrect datatype on ruleset %d\n", ruleSetArray[i].inputId);
				printf("Ruleset Datatype %d Datatype %d \n", ruleSetArray[i].datatype, data->datatype);
				return;
			}	 
			// Apply entire RuleSet on data

			IG_Verarbeiter_applyRule(data, &ruleSetArray[i]);
			break;
		}				
	}

}


void IG_Verarbeiter_checkIntervals(IG_Input_RuleSet * ruleSetArray, IG_UInt32 ruleSetSize, IG_Queue * queue){

	// Go through every rule and check if to Send
	IG_DateTime now = IG_DateTime_now();
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		IG_Input_RuleSet * ruleSet = &(ruleSetArray[i]);
		for(IG_UInt32 j = 0; j < ruleSet->ruleSize; j++){
			if(ruleSet->rules[j].deadline < now){
				IG_Rule* rule = &(ruleSet->rules[j]);

				// Check if values arent null
				if(rule->data==NULL){
					continue;
				}

				IG_Data * dataToSend = malloc(sizeof(IG_Data));
				dataToSend->id = rule->outputId;
				dataToSend->datatype = IG_CHAR;
				dataToSend->data = (void*)IG_Verarbeiter_encodeToJSON(rule->data);
				dataToSend->timestamp = IG_DateTime_now();
				IG_Queue_put(queue, dataToSend);
				rule->deadline = IG_DateTime_add_duration(rule->deadline, rule->interval);

				IG_Data_delete_members(rule->data);
				IG_Data_delete(rule->data);
				rule->data = NULL;
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
			printf("ID: %i\n", rule->outputId);
			switch(rule->rule){
				case IG_RULE_TRANSMIT:
					rule->function = &IG_Transmit;
					break;
				case IG_RULE_AVG:
					rule->function = &IG_Average;
					break;
				case IG_RULE_MAX:
					rule->function = &IG_Maximum;
					break;
				case IG_RULE_MIN:
					rule->function = &IG_Minimum;
					break;
				default:
					printf("ERROR\n");
					break;
			}
			// Init times also
			rule->deadline= IG_DateTime_add_duration(IG_DateTime_now() ,rule->interval);			
		}
	}
}

IG_Char* IG_Verarbeiter_encodeToJSON(IG_Data* data){
	unsigned char* s = (unsigned char *) malloc(sizeof(unsigned char) * 40);
	
	snprintf(s, 40, "{\"value\": \"%s\"}", IG_Data_toString(data));

	return (IG_Char*)s;
}

void IG_Transmit(IG_Data* data, IG_Rule * rule){
	if(rule->data == NULL){
		rule->data = IG_Data_copy(data);
		return;
	}

	IG_Data_delete_members(rule->data);
	IG_Data_delete(rule->data);

	rule->data = IG_Data_copy(data);
}


void IG_Average(IG_Data* data, IG_Rule * rule){

	// Check if this is the first ever data
	if(rule->data==NULL){
		rule->data = IG_Data_copy(data);
		rule->size = 1;
		return;
	}

	// Formula is avg(new) = avg(old) + ( (value(new)-avg(old)) / size(new));
	switch(data->datatype){
		case IG_FLOAT:
			rule->size++;
			*((IG_Float*)rule->data->data) = *((IG_Float*)rule->data->data) + 
				(( *((IG_Float*)data->data) - *((IG_Float*)rule->data->data) ) / rule->size);
			break;
		case IG_DOUBLE:
			rule->size++;
			*((IG_Double*)rule->data->data) = *((IG_Double*)rule->data->data) + 
				(( *((IG_Double*)data->data) - *((IG_Double*)rule->data->data) ) / (IG_Double) rule->size);
			break;
		case IG_INT32:
			rule->size++;
			*((IG_Int32*)rule->data->data) = *((IG_Int32*)rule->data->data) + 
				(( *((IG_Int32*)data->data) - *((IG_Int32*)rule->data->data) ) / rule->size);
			break;
		case IG_UINT32:
			rule->size++;
			*((IG_UInt32*)rule->data->data) = *((IG_UInt32*)rule->data->data) + 
				(( *((IG_UInt32*)data->data) - *((IG_UInt32*)rule->data->data) ) / rule->size);
			break;
		case IG_INT64:
			rule->size++;
			*((IG_Int64*)rule->data->data) = *((IG_Int64*)rule->data->data) + 
				(( *((IG_Int64*)data->data) - *((IG_Int64*)rule->data->data) ) / rule->size);
			break;
		case IG_UINT64:
		case IG_DURATION:
		case IG_DATETIME:
			rule->size++;
			*((IG_UInt64*)rule->data->data) = *((IG_UInt64*)rule->data->data) + 
				(( *((IG_UInt64*)data->data) - *((IG_UInt64*)rule->data->data) ) / rule->size);
			break;
		case IG_BYTE:
			rule->size++;
			*((IG_Byte*)rule->data->data) = *((IG_Byte*)rule->data->data) + 
				(( *((IG_Byte*)data->data) - *((IG_Byte*)rule->data->data) ) / rule->size);
			break;
		case IG_CHAR:
			rule->size++;
			*((IG_Char*)rule->data->data) = *((IG_Char*)rule->data->data) + 
				(( *((IG_Char*)data->data) - *((IG_Char*)rule->data->data) ) / rule->size);
			break;
		case IG_BOOL:
			rule->size++;
			*((IG_Bool*)rule->data->data) = *((IG_Bool*)rule->data->data) + 
				(( *((IG_Bool*)data->data) - *((IG_Bool*)rule->data->data) ) / rule->size);
			break;
		default:
			break;
	}
}
void IG_Maximum(IG_Data* data, IG_Rule * rule){

	// Check if this is the first ever data
	if(rule->data==NULL){
		rule->data = IG_Data_copy(data);
		return;
	}

	switch(data->datatype){
		case IG_FLOAT:	
			if(*((IG_Float*)rule->data->data) > *((IG_Float*)data->data)) 
				return;

			*((IG_Float*)rule->data->data) = *((IG_Float*)data->data);
			break;
		case IG_DOUBLE:	
			if(*((IG_Double*)rule->data->data) > *((IG_Double*)data->data)) 
				return;

			*((IG_Double*)rule->data->data) = *((IG_Double*)data->data);
			break;
		case IG_INT32:		
			if(*((IG_Int32*)rule->data->data) > *((IG_Int32*)data->data)) 
				return;

			*((IG_Int32*)rule->data->data) = *((IG_Int32*)data->data);
			break;
		case IG_UINT32:	
			if(*((IG_UInt32*)rule->data->data) > *((IG_UInt32*)data->data)) 
				return;

			*((IG_UInt32*)rule->data->data) = *((IG_UInt32*)data->data);
			break;
		case IG_INT64:					
			if(*((IG_Int64*)rule->data->data) > *((IG_Int64*)data->data)) 
				return;

			*((IG_Int64*)rule->data->data) = *((IG_Int64*)data->data);
			break;	
		case IG_UINT64:
		case IG_DURATION:
		case IG_DATETIME:	
			if(*((IG_UInt64*)rule->data->data) > *((IG_UInt64*)data->data)) 
				return;

			*((IG_UInt64*)rule->data->data) = *((IG_UInt64*)data->data);
			break;	
		case IG_BYTE:		
			if(*((IG_Byte*)rule->data->data) > *((IG_Byte*)data->data)) 
				return;

			*((IG_Byte*)rule->data->data) = *((IG_Byte*)data->data);
			break;
		case IG_CHAR:		
			if(*((IG_Char*)rule->data->data) > *((IG_Char*)data->data)) 
				return;

			*((IG_Char*)rule->data->data) = *((IG_Char*)data->data);
			break;
		case IG_BOOL:		
			if(*((IG_Bool*)rule->data->data) > *((IG_Bool*)data->data)) 
				return;

			*((IG_Bool*)rule->data->data) = *((IG_Bool*)data->data);
		default:
			break;
	}
}
void IG_Minimum(IG_Data* data, IG_Rule * rule){

	// Check if this is the first ever data
	if(rule->data==NULL){
		rule->data = IG_Data_copy(data);
		return;
	}

	switch(data->datatype){
		case IG_FLOAT:	
			if(*((IG_Float*)rule->data->data) < *((IG_Float*)data->data)) return;
			*((IG_Float*)rule->data->data) = *((IG_Float*)data->data);
			break;
		case IG_DOUBLE:	
			if(*((IG_Double*)rule->data->data) < *((IG_Double*)data->data)) return;
			*((IG_Double*)rule->data->data) = *((IG_Double*)data->data);
			break;
		case IG_INT32:		
			if(*((IG_Int32*)rule->data->data) < *((IG_Int32*)data->data)) return;
			*((IG_Int32*)rule->data->data) = *((IG_Int32*)data->data);
			break;
		case IG_UINT32:	
			if(*((IG_UInt32*)rule->data->data) < *((IG_UInt32*)data->data)) return;
			*((IG_UInt32*)rule->data->data) = *((IG_UInt32*)data->data);
			break;
		case IG_INT64:					
			if(*((IG_Int64*)rule->data->data) < *((IG_Int64*)data->data)) return;
			*((IG_Int64*)rule->data->data) = *((IG_Int64*)data->data);
			break;	
		case IG_UINT64:
		case IG_DURATION:
		case IG_DATETIME:	
			if(*((IG_UInt64*)rule->data->data) < *((IG_UInt64*)data->data)) return;
			*((IG_UInt64*)rule->data->data) = *((IG_UInt64*)data->data);
			break;	
		case IG_BYTE:		
			if(*((IG_Byte*)rule->data->data) < *((IG_Byte*)data->data)) return;
			*((IG_Byte*)rule->data->data) = *((IG_Byte*)data->data);
			break;
		case IG_CHAR:		
			if(*((IG_Char*)rule->data->data) < *((IG_Char*)data->data)) return;
			*((IG_Char*)rule->data->data) = *((IG_Char*)data->data);
			break;
		case IG_BOOL:		
			if(*((IG_Bool*)rule->data->data) < *((IG_Bool*)data->data)) return;
			*((IG_Bool*)rule->data->data) = *((IG_Bool*)data->data);
		default:
			break;
	}
}


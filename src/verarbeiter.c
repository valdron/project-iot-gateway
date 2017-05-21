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
	
	workLoopArgs args = (workLoopArgs){verarbeiter, ruleSets, size};
	pthread_t thread;	

	pthread_create(&thread,NULL,workLoop,(void*)&args)

	return IG_GOOD;
}

void* workLoop(void*){
	
}

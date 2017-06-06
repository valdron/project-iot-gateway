#include "verarbeiter.h"
#include<unistd.h>
#include <time.h>


int main(int argc, char* argv[]){
	
	srand(time(NULL));
	

	// Setup config
	printf("Initiating config\n");	
	IG_Config* configVerarbeiter = IG_Config_create(argv[1], IG_CONFIG_VERARBEITER);
	IG_Config* configErfasser = IG_Config_create(argv[2], IG_CONFIG_OPC);
	IG_Config* configSender = IG_Config_create(argv[3], IG_CONFIG_MQTT);

	// Setup versender
	printf("Initiating sender\n");
	IG_Datenversender * sender = IG_Datenversender_create(configSender);

	// Setup erfasser and fill its queue with Data
	printf("Initiating erfasser\n");
	IG_Datenerfasser * erfasser = IG_Datenerfasser_create_nonBlocking(configErfasser);

	printf("Creating Data\n");
	for(int i = 0; i<100; i+=1.0){
		

		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		if(data==NULL) return EXIT_FAILURE;

		void* record;

		if(rand()%2==1){
			data->id = 1;
			data->datatype = IG_DOUBLE;

			record = malloc(sizeof(IG_Double));
			if(record==NULL) return EXIT_FAILURE;

			*((IG_Double*)record) = (IG_Double)i*i;
		}else{
			data->id = 4;
			data->datatype = IG_UINT32;

			record = malloc(sizeof(IG_UInt32));
			if(record==NULL) return EXIT_FAILURE;

			*((IG_UInt32*)record) = (IG_UInt32)(i*i);
		}

				
		data->data = (void*)record;
		data->timestamp = 0;
		// Inserting into the queue
		IG_Queue_put(erfasser->queue,data);
	}

	printf("Creating verarbeiter\n");
	IG_Verarbeiter* verarbeiter = IG_Verarbeiter_create(configVerarbeiter, sender, erfasser);
	
	if(verarbeiter == NULL) printf("IG_Verarbeiter_create failed");

	printf("Initiating verarbeiter\n");
	IG_Status status = IG_Verarbeiter_init(verarbeiter);
	if(status!=IG_STATUS_GOOD) {
		printf("IG_Verarbeiter_init failed");
		return -1;
	}

	// After starting the verarbeiter we just wait and watch the output
	printf("Starting loop\n");
	while(1){
		if(IG_Queue_isEmpty(sender->queue)) continue;
		IG_Data* data = IG_Queue_take(sender->queue);
		printf("Recieved JSON String: %s for OutID: %d \n", (IG_Char*)data->data, data->id);
		sleep(2);
	}
	

	return 0;
}

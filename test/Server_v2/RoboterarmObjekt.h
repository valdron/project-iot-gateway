void addRoboterarmObjectInstance(UA_Server *server, char *name);
void addRoboterarmTypeConstructor(UA_Server *server);
void defineObjectTypes(UA_Server *server);
void initVariables();
void *calcCurrentPressure(void *serverPtr);
void *calcCurrentTemp(void *serverPtr);
void *askForState(void *serverPtr);
void addRoboterarmObjectInstance(UA_Server *server, char *name);
void addRoboterarmTypeConstructor(UA_Server *server);
void defineObjectTypes(UA_Server *server);
void initVariants();
void *calcCurrentPressure(void *value);
void *calcCurrentTemp(void *value);

//for datatypes
#include "internals.h"

// returns next Data or returns IG_DATA_EMPTY;
// fetches the data from queue
IG_Data getNextData();

// returns if there is any new data in the queue
// should be called before getNextData();
bool anyNewData();


// initializes the connection to the Datasource(OPC UA) and starts the extra thread with loop over incoming data
// TODO: needs config parameters (as pointer?) 
// TODO: parameter for flags?
IG_Status init(/*...*/);


//for datatypes
#include "internals.h"

// returns next Data if there is any;
// fetches the data from queue
IG_Data getNextData();


// initializes the connection to the Datasource(OPC UA) and starts the extra thread with loop over incoming data
// TODO: needs config parameters (as pointer?) 
// TODO: parameter for flags?
IG_Status init(/*...*/);

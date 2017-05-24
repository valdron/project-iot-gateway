
#ifndef STARTER_H
#define STARTER_H

// This file is for setting up all the components and initializing them to be started
// It should read the configuration from the files aswell
// It should make sure that everything is started in the right order

#include "internals.h"
#include "datenerfasser.h"
#include "datenversender.h"
#include "verarbeiter.h"
#include "configuration.h"


// the main function of this component
// should be called once at startup from main() and should block until the gateway is shutdown
// TODO: which parameters will be needed at startup? config filename(s)?

//weitere Threads hinzufügen
IG_Status run(char * filename);

#endif
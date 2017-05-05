// main function
// argument parsing
// calling run(...)

#include<stdio.h>
#include<string.h>

//TODO: documentation about arguments

#define USAGE "USAGE: \nIoTGateway -f [config-file]\n"

#include "starter.h"

int main(int argc, char * argv[]) {


    // parse & check arguments
    if(argc != 3) {
        printf(USAGE);
        return -1;
    }
    char * config_filename; 
    if(!strcmp("-f",argv[0])){
        printf(USAGE);
        return -1;
    } else {
        config_filename = argv[1];
    }

    // call run with parameters
    run(config_filename);

    // shutdown notice
    
}




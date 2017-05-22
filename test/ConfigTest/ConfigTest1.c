
#include<stdio.h>
#include "configuration.h"

#define USAGE "command [xmlfile]"

int main(int argc, char * argv[]) {

    if(argc != 2) {
        printf(USAGE);
        return -1;
    }

    IG_Config * conf = IG_Config_create(argv[1], IG_CONFIG_STANDARD);

    if(conf == NULL) {
        printf("couldnot parse string\n");
        return -1;
    }

    IG_Config_delete(conf);

    printf("everything worked!\n");
    return 0;
}
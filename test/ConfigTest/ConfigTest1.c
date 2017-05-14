
#include<stdio.h>
#include "configuration.h"

#define FILENAME "test_standard.xml"

int main(void) {
    IG_Config * conf = IG_Config_create(FILENAME, IG_CONFIG_STANDARD);
    if(conf == NULL) {
        printf("couldnot parse file: %s (this is bad)\n",FILENAME);
        return -1;
    }

    IG_Config_delete(conf);

    printf("everything worked!\n");
    return 0;
}
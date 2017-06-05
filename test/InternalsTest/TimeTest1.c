#include "internals.h"
#include<time.h>
#include<stdio.h>

int main(int argc, char * argv[]) {

    IG_DateTime now = IG_DateTime_now();
    time_t t = time(NULL);

    printf("time in s since 1970: '%ld'\t\ttime in 100ns since 1601: '%ld'\n",t,now);

    if(((now - 116444736000000000L)/10000000L) != t) {
        printf("failed\n");
        return -1;
    }

    return 0;

}
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include "queue.h"
#include "internals.h"

void * threadonefunc(void * );
void * threadtwofunc(void * );

int main(void) {

    printf("Main: started\n");
    IG_Queue * queue = IG_Queue_new(IG_QUEUE_BLOCKING);
    
    pthread_t threadone;
    pthread_t threadtwo;

    printf("Main: Creating threads\n");
    pthread_create(&threadone, NULL, threadonefunc, queue);
    pthread_create(&threadtwo, NULL, threadtwofunc, queue);

    printf("Main: waiting for threads\n");
    int rt;
    void * ret = (void*)&rt;
    pthread_join(threadtwo,&ret);
    printf("Main: Thread2 returned\n");
    pthread_join(threadone,&ret);
    printf("Main: Thread1 returned\n");

}

void * threadonefunc(void * arg) {
    IG_Queue *queue = (IG_Queue *) arg;
    printf("Thread1: started\n");
    printf("Thread1: trying to take data\n");
    IG_Data * data = IG_Queue_take(queue);
    printf("Thread1: take() returned\n");
    if(data == NULL)
        printf("Thread1: take() returned NULL\n");
    else    
        printf("Thread1: take() returned a valid struct\n");
    printf("Thread1: exiting\n");
    pthread_exit(NULL);
}

void * threadtwofunc(void * arg) {
    IG_Queue *queue = (IG_Queue *) arg;
    printf("Thread2: started\n");
    printf("Thread2: sleeping for 1s\n");
    sleep(1);
    printf("Thread2: woken after 1s\n");
    IG_Data * data = IG_Data_create(0, IG_NULL, NULL, 0);
    IG_Queue_put(queue, data);
    printf("Thread2: putting in queue\n");
    printf("Thread2: exiting\n");
    pthread_exit(NULL);
}

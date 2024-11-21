#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "procedure.h"


#define NUM_THREADS     10

int main()
{
    pthread_t threads[NUM_THREADS];

    Buffer *buff = (Buffer *) malloc(sizeof(Buffer));

    buff->val = 0;
    buff->num_scrittori_in_attesa = 0;
    buff->numlettori = 0;
    buff->numscrittori = 0;

    pthread_mutex_init(&(buff->mutex),NULL);
    pthread_cond_init(&(buff->cond_lettori),NULL);
    pthread_cond_init(&(buff->cond_scrittori),NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (i%2 == 0) pthread_create(&threads[i],NULL,lettore,(void *)buff);
        else pthread_create(&threads[i],NULL,scrittore,(void *)buff);
    }

    for (int i = 0; i < NUM_THREADS; i++) pthread_join(threads[i],NULL);

    pthread_mutex_destroy(&(buff->mutex));
    pthread_cond_destroy(&(buff->cond_lettori));
    pthread_cond_destroy(&(buff->cond_scrittori));

    free(buff);

    pthread_exit(0);
}
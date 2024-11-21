#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <pthread.h>


typedef struct
{
    int val;

    int numscrittori;
    int numlettori;

    int num_scrittori_in_attesa; //qui non abbiamo la queue_condition, ci servono per garantire una starvation equilibrata

    pthread_mutex_t mutex;
    pthread_cond_t cond_scrittori;
    pthread_cond_t cond_lettori;


}Buffer;

void *lettore(void *d);
void *scrittore(void *d);

void leggi(Buffer *p);
void scrivi(Buffer *p);

#endif
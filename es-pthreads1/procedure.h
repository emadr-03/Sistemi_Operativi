#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <pthread.h>
#include <sys/types.h>


typedef struct
{
    int val;

    pthread_mutex_t mutex; //per avere il mutex di accesso al monitor

    pthread_cond_t ok_produttore; //variabili condition 
    pthread_cond_t ok_consumatore;


    int buffer_libero;
}Buffer;


//starting routines dei thread
void *produttore(void *d);
void *consumatore(void *d);

void produci(Buffer *p,int valore);
int consuma(Buffer *p);

#endif
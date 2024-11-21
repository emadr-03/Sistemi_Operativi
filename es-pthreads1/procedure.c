#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "procedure.h"

void produci(Buffer *p,int valore)
{
    //inizio produzione
    pthread_mutex_lock(&(p->mutex)); //entro nel monitor

    while (p->buffer_libero == 0) pthread_cond_wait(&(p->ok_produttore),&(p->mutex)); //se il buffer è pieno attendo

    p->val = valore;


    //fine produzione
    p->buffer_libero = 0;

    pthread_cond_signal(&(p->ok_consumatore));

    pthread_mutex_unlock(&(p->mutex));
}

int consuma(Buffer *p)
{
    pthread_mutex_lock(&(p->mutex));

    while (p->buffer_libero == 1) pthread_cond_wait(&(p->ok_consumatore),&(p->mutex));

    int valore;
    valore = p->val;

    p->buffer_libero = 1;

    pthread_cond_signal(&(p->ok_produttore));

    pthread_mutex_unlock(&(p->mutex));

    return valore;
}

void *produttore(void *d)
{
    Buffer *p = (Buffer *) d;
    srand(getpid());
    int valore;

    for (int i = 0; i < 5; i++)
    {
        valore = rand() % 81;
        produci(p,valore);

        printf("[Thread Produttore] Valore prodotto: %d\n",p->val);
    }
    pthread_exit(NULL);
}

void *consumatore(void *d)
{
    Buffer *p = (Buffer *) d;
    int val;
    
    for (int i = 0; i < 5; i++)
    {
        val = consuma(p);
        printf("[Thread Consumatore] Valore consumato: %d\n",val);
    }
    pthread_exit(NULL);
}
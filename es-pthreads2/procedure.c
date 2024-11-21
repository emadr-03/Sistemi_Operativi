#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "procedure.h"

void scrivi(Buffer *p)
{
    pthread_mutex_lock(&(p->mutex));

    while (p->numlettori > 0 || p->numscrittori > 0)
    {
        p->num_scrittori_in_attesa++;
        pthread_cond_wait(&(p->cond_scrittori),&(p->mutex));
        p->num_scrittori_in_attesa--;
    }

    p->numscrittori++;

    pthread_mutex_unlock(&(p->mutex));

    int valore;
    srand(getpid());
    p->val = valore;

    printf("[Scrittore] Valore scritto: %d\n",p->val);

    pthread_mutex_lock(&(p->mutex));

    p->numscrittori--;

    if (p->num_scrittori_in_attesa > 0) pthread_cond_signal(&(p->cond_scrittori));
    else pthread_cond_broadcast(&(p->cond_lettori));

    pthread_mutex_unlock(&(p->mutex));
}

int leggi(Buffer *p)
{
    pthread_mutex_lock(&(p->mutex));

    while (p->numscrittori > 0) pthread_cond_wait(&(p->cond_lettori),&(p->mutex));

    p->numlettori++;

    pthread_mutex_unlock(&(p->mutex));

    int valore;
    valore = p->val;

    pthread_mutex_lock(&(p->mutex));

    p->numlettori--;

    if (p->numlettori == 0) pthread_cond_signal(&(p->cond_scrittori));

    pthread_mutex_unlock(&(p->mutex));

    return valore;
}

void *lettore(void *d)
{
    Buffer *p = (Buffer *) d;
    int valore;

    for (int i = 0; i < 5; i++)
    {
        valore = leggi(p);
        printf("[Lettore] Valore letto: %d\n",valore);
    }
    pthread_exit(0);
}

void *scrittore(void *d)
{
    Buffer *p = (Buffer *) d;

    for (int i = 0; i < 5; i++) scrivi(p);

    pthread_exit(0);
}
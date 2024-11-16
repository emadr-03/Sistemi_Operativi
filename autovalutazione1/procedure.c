#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "procedure.h"
#include "semafori.h"

void iniziolettura(Buffer *p,int semid)
{
    Wait_Sem(semid,MUTEXL);

    p->numlettori++;

    if (p->numlettori == 1) Wait_Sem(semid,SYNCH);

    Signal_Sem(semid,MUTEXL);
}

void finelettura(Buffer *p,int semid)
{
    Wait_Sem(semid,MUTEXL);
    
    p->numlettori--;

    if (p->numlettori == 0) Signal_Sem(semid,SYNCH);

    Signal_Sem(semid,MUTEXL);
}

void lettore_elaboratore(Buffer *p,int *len,int semid)
{
    iniziolettura(p,semid);

    char converti[*len];

    for (int i = 0; i < *len; i++)
    {
        converti[i] = p->stringa[i] - 32;
    }

    converti[*len] = '\0';

    printf("[Elaboratore] Conversione stringa da '%s' a '%s'\n",p->stringa,converti);

    finelettura(p,semid);
}

void lettore_analizzatore(Buffer *p,int *len,int semid)
{
    iniziolettura(p,semid);

    char inverti[*len];

    for (int i = 0; i < *len; i++)
    {
        inverti[i] = p->stringa[*(len)-i-1];
    }

    inverti[*len] = '\0';

    printf("[Analizzatore] Stringa invertita da '%s' a '%s'\n",p->stringa,inverti);

    finelettura(p,semid);
}

void scrittore(Buffer *p,int *len,int semid)
{
    Wait_Sem(semid,SYNCH);

    char carattere;
    srand(time(NULL)*getpid());

    for (int i = 0; i < *len; i++)
    {
        carattere = 'a' + (rand() % 26);
        p->stringa[i] = carattere;
    }

    p->stringa[*len] = '\0';

    sleep(1);
    printf("[Generatore] Stringa generata: %s\n",p->stringa);

    Signal_Sem(semid,SYNCH);
}
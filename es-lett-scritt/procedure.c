#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"

void iniziolettura(int semid,Buffer *p)
{
    Wait_Sem(semid,SYNCH);
}

void finelettura(int semid,Buffer *p)
{
    Signal_Sem(semid,SYNCH);
}

void lettore(int semid,Buffer *p)
{
    iniziolettura(semid,p);

    sleep(2);
    printf("[Lettore] Valore letto (var1): %d\n",p->val1);
    printf("[Lettore] Valore letto (var2): %d\n",p->val2);

    finelettura(semid,p);
}

void inizioscrittura(int semid,Buffer *p)
{
    Wait_Sem(semid,SYNCH);
    Wait_Sem(semid,MUTEXS);
}

void finescrittura(int semid,Buffer *p)
{
    Signal_Sem(semid,SYNCH);
    Signal_Sem(semid,MUTEXS);
}

void scrittore(int semid,Buffer *p)
{
    inizioscrittura(semid,p);

    srand(time(NULL) * getpid());
    
    p->val1 = rand() % 10;
    p->val2 = rand() % 10;
    sleep(1);
    printf("[Scrittore] Valore scritto (var1): %d\n",p->val1);
    printf("[Scrittore] Valore scritto (var2): %d\n",p->val2);

    finescrittura(semid,p);
}
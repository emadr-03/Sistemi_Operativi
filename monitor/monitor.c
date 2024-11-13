#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "monitor.h"

static void Wait_Sem(int semid,int numsem);
static void Signal_Sem(int semid,int numsem);


void initmonitor(Monitor *m,int num_var)
{
    //inizializzazione mutex del monitor
    m->mutex = semget(IPC_PRIVATE,0,IPC_CREAT|0664);
    semctl(m->mutex,0,SETVAL,1);

    //inizializzazione semafori per ogni variabile condition
    m->semcond = semget(IPC_PRIVATE,num_var,IPC_CREAT|0664);
    for (int i = 0; i < num_var; i++) semctl(m->semcond,i,SETVAL,0);

    //init semaforo urgenti
    m->semurg = semget(IPC_PRIVATE,0,IPC_CREAT|0664);
    semctl(m->semurg,0,SETVAL,0);

    m->num_var_cond = num_var;

    //init shared memory var condition + coda urgenti
    m->shmid = shmget(IPC_PRIVATE,sizeof(int)*(num_var+1),IPC_CREAT|0664);
    m->cond_counts = (int *) shmat(m->shmid,0,0);
    m->urg_counts = m->cond_counts + m->num_var_cond;

    //init valori della shm
    for (int i = 0; i < num_var+1; i++) m->cond_counts[i] = 0;
    *(m->urg_counts) = 0;
}

void entermonitor(Monitor *m)
{
    Wait_Sem(m->mutex,0);
}

void leavemonitor(Monitor *m)
{
    if (*(m->urg_counts) > 0) Signal_Sem(m->semurg,0);
    else Signal_Sem(m->mutex,0);
}

void removemonitor(Monitor *m)
{
    semctl(m->mutex,0,IPC_RMID);
    semctl(m->semcond,0,IPC_RMID);
    semctl(m->semurg,0,IPC_RMID);
    shmctl(m->shmid,IPC_RMID,0);
}

void Wait_Cond(Monitor *m,int numvar)
{
    //il processo che fa la wait incrementa il contatore
    m->cond_counts[numvar]++;

    //se ci sono dei processi nella coda degli urgenti fai entrare loro, altrimenti fai entrare il prossimo processo normalmente
    if (*(m->urg_counts) > 0) Signal_Sem(m->semurg,0);
    else Signal_Sem(m->mutex,0);

    //mettiti in attesa sul semaforo condition
    Wait_Sem(m->semcond,numvar);
    m->cond_counts[numvar]--;
}

void Signal_Cond(Monitor *m,int numvar)
{
    //chi libera il semaforo condition (segnalando che la risorsa è ora disponibile) incrementa il numero dei processi urgenti
    *(m->urg_counts)++;

    if (m->cond_counts[numvar] > 0) //se ci sono dei processi sul semaforo della var condition
    {
        Signal_Sem(m->semcond,numvar); //fai entrare il processo che stava aspettando la risorsa
        Wait_Sem(m->semurg,0); //il processo si mette nella coda degli urgenti
    }
    //altrimenti, non succede niente

    *(m->urg_counts)--;
}

//implementazione semafori

void Wait_Sem(int semid,int numsem)
{
    struct sembuf sem_buf;
    sem_buf.sem_num = numsem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = -1;

    semop(semid,&sem_buf,1);
}

void Signal_Sem(int semid,int numsem)
{
    struct sembuf sem_buf;
    sem_buf.sem_num = numsem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = 1;

    semop(semid,&sem_buf,1);
}
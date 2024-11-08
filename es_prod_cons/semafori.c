#include <stdio.h>
#include <errno.h>

#include "semafori.h"

int Wait_Sem(int semid,int numsem)
{
    int err;
    struct sembuf sem_buf;

    sem_buf.sem_num = numsem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = -1;

    err = semop(semid,&sem_buf,1);

    if (err < 0) printf("Errore WAIT\n");

    return err;
}

int Signal_Sem(int semid,int numsem)
{
    int err;
    struct sembuf sem_buf;

    sem_buf.sem_num = numsem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = 1;

    err = semop(semid,&sem_buf,1);

    if (err < 0) printf("Errore SIGNAL\n");

    return err;
}
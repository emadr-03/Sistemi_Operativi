#include "semafori.h"
#include <stdio.h>

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
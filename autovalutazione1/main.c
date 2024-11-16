#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "procedure.h"

int main()
{
    key_t chiavelen = ftok(".",'a');

    int shmlen = shmget(chiavelen,sizeof(int),IPC_CREAT|IPC_EXCL|0664);

    if (shmlen < 0)
    {
        printf("Risorsa gia' esistente\n");
        shmlen = shmget(chiavelen,sizeof(int),0);
        shmctl(shmlen,IPC_RMID,0);
        exit(0);
    }

    int *len = (int *) shmat(shmlen,0,0);

    srand(time(NULL)*getpid());

    *len = (rand() % 6) + 10;

    key_t chiavestringa = ftok(".",'b');
    int shmstringa = shmget(chiavestringa,sizeof(Buffer),IPC_CREAT|IPC_EXCL|0664);

    if (shmstringa < 0)
    {
        printf("Risorsa gia' esistente\n");
        shmstringa = shmget(chiavestringa,sizeof(Buffer),0);
        shmctl(shmstringa,IPC_RMID,0);
        exit(0);
    }

    Buffer *buff = (Buffer *) shmat(shmstringa,0,0);

    key_t chiavesemafori = ftok(".",'c');
    int semid = semget(chiavesemafori,2,IPC_CREAT|IPC_EXCL|0664);

    if (semid < 0)
    {
        printf("Risorsa gia' esistente\n");
        semid = semget(chiavesemafori,2,0);
        semctl(semid,0,IPC_RMID);
        exit(0);
    }

    semctl(semid,MUTEXL,SETVAL,1);
    semctl(semid,SYNCH,SETVAL,1);

    //creazione generatore
    pid_t pid1 = fork();

    if (pid1 == 0)
    {
        execl("./generatore","generatore",NULL);
        exit(0);
    }

    //creazione elaboratore
    pid_t pid2 = fork();

    if (pid2 == 0)
    {
        execl("./elaboratore","elaboratore",NULL);
        exit(0);
    }

    //creazione analizzatori
    pid_t pid3 = fork();

    if (pid3 == 0)
    {
        execl("./analizzatore","analizzatore",NULL);
        exit(0);
    }

    pid_t pid4 = fork();

    if (pid4 == 0)
    {
        execl("./analizzatore","analizzatore",NULL);
        exit(0);
    }

    for (int i = 0; i < 4; i++) wait(NULL);

    kill(pid1,SIGKILL);
    kill(pid2,SIGKILL);
    kill(pid3,SIGKILL);
    kill(pid4,SIGKILL);

    shmctl(shmlen,IPC_RMID,0);
    shmctl(shmstringa,IPC_RMID,0);
    semctl(semid,0,IPC_RMID);

    return 0;
}
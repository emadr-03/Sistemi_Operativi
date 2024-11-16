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
}
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
    int shmlen = shmget(chiavelen,sizeof(int),0);

    int *len = (int *) shmat(shmlen,0,0);

    key_t chiavestringa = ftok(".",'b');
    int shmstringa = shmget(chiavestringa,sizeof(Buffer),0);

    Buffer *buff = (Buffer *) shmat(shmstringa,0,0);

    key_t chiavesemafori = ftok(".",'c');
    int semid = semget(chiavesemafori,2,0);

    while(1)
    {
        scrittore(buff,len,semid);
        sleep(3);
    }

    return 0;
}
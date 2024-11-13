#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "procedure.h"

int main()
{
    int shmid = shmget(IPC_PRIVATE,sizeof(prodcons),IPC_CREAT|0664);
    prodcons *buff;
    buff = (prodcons *) shmat(shmid,0,0);

    buff->buffer = 0;
    buff->buffer_libero = 1;
    initmonitor(&(buff->m),2);

    for (int i = 0; i < NUM_CONSUMATORI; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            consumatore(buff);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_PRODUTTORI; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            produttore(buff);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_CONSUMATORI+NUM_PRODUTTORI; i++) wait(NULL);

    removemonitor(&(buff->m));

    shmctl(shmid,IPC_RMID,0);

    return 0;
}
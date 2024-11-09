#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "procedure.h"

int main()
{
    key_t chiave_shm_buff = ftok(".",'a');

    int shm_buff = shmget(chiave_shm_buff,sizeof(Buffer),IPC_CREAT|IPC_EXCL|0664);

    if (shm_buff < 0)
    {
        printf("Risorsa buffer gia' esistente\n");
        shm_buff = shmget(chiave_shm_buff,sizeof(Buffer),0);
        shmctl(shm_buff,IPC_RMID,0);
        exit(0);
    }

    Buffer *buff;
    buff = (Buffer *) shmat(shm_buff,0,0);

    buff->num_lettori = 0;
    buff->num_scrittori = 0;
    buff->val1 = 0;
    buff->val2 = 0;

    key_t chiave_semafori = ftok(".",'b');

    int semid = semget(chiave_semafori,3,IPC_CREAT|IPC_EXCL|0664);

    if (semid < 0)
    {
        printf("Risorsa semaforo gia' esistente\n");
        semid = semget(chiave_semafori,3,0);
        semctl(semid,0,IPC_RMID);
        exit(0);
    }

    semctl(semid,MUTEX,SETVAL,1);
    semctl(semid,SYNCH,SETVAL,1);
    semctl(semid,MUTEXS,SETVAL,1);

    for (int i = 0; i < NUM_SCRITTORI+NUM_LETTORI; i++)
    {
        pid_t pid = fork();
        
        if (pid == 0)
        {
            if ((i%2) == 0)
            {
                execl("./scrittore","scrittore",NULL);
            }
            else
            {
                execl("./lettore","lettore",NULL);
            }
            exit(0);
        }
    }

    for (int i = 0; i < NUM_SCRITTORI+NUM_LETTORI; i++) wait(NULL);

    shmctl(shm_buff,IPC_RMID,0);
    semctl(semid,0,IPC_RMID);

    return 0;
}
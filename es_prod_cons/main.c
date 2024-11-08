#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "semafori.h"
#include "procedure.h"

int main()
{
    key_t chiave_shm_posti = ftok(".",'a');

    int shm_id_posti = shmget(chiave_shm_posti,sizeof(posto)*DIM,IPC_CREAT|0664);
    int shm_id_disp = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);

    posto *buffer_posti;
    buffer_posti = (posto *) shmat(shm_id_posti,0,0);

    int *buffer_disp;
    buffer_disp = (int *) shmat(shm_id_disp,0,0);
    *buffer_disp = DIM;

    for (int i = 0; i < DIM; i++)
    {
        buffer_posti[i].id_cliente = 0;
        buffer_posti[i].stato = 0;
    }

    int semid = semget(IPC_PRIVATE,1,IPC_CREAT|0664);
    semctl(semid,MUTEX,SETVAL,1);

    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            printf("[Cliente] PID: %d\n",getpid());
            prenotazione(semid,buffer_disp,buffer_posti,getpid());
            exit(0);
        }
    }

    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        wait(NULL);
    }

    execl("./visualizzatore","visualizzatore",NULL);

    shmctl(shm_id_disp,IPC_RMID,0);
    shmctl(shm_id_posti,IPC_RMID,0);
    semctl(semid,MUTEX,IPC_RMID);

    return 0;
}
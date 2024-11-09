#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "semafori.h"
#include "procedure.h"

int main()
{
    key_t chiave_shm_posti = ftok(".",'a');

    int shm_id_posti = shmget(chiave_shm_posti,sizeof(posto)*DIM,IPC_CREAT|IPC_EXCL|0664);

    if (shm_id_posti < 0)
    {
        printf("Risorsa posti gia' esistente\n");
        shm_id_posti = shmget(chiave_shm_posti,sizeof(posto)*DIM,0);
        shmctl(shm_id_posti,IPC_RMID,0);
        exit(0);
    }

    int shm_id_disp = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);

    int shm_id_termina = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);

    posto *buffer_posti;
    buffer_posti = (posto *) shmat(shm_id_posti,0,0);

    int *buffer_disp;
    buffer_disp = (int *) shmat(shm_id_disp,0,0);
    *buffer_disp = DIM;

    for (int i = 0; i < DIM; i++)
    {
        buffer_posti[i].id_cliente = 0;
        buffer_posti[i].stato = POSTO_LIBERO;
    }

    int semid = semget(IPC_PRIVATE,1,IPC_CREAT|0664);
    semctl(semid,MUTEX,SETVAL,1);

    pid_t pid1 = fork();
    if (pid1 == 0)
    {
        execl("./visualizzatore","visualizzatore",NULL);
    }

    int num_prenotazioni;
    int tempo_sleep;
    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            srand(time(NULL)*getpid());
            num_prenotazioni = (rand() % 4) + 1;
            tempo_sleep = rand() % 6;
            Wait_Sem(semid,MUTEX);
            sleep(tempo_sleep);
            for (int j = 0; j < num_prenotazioni; j++)
            {
                if ((*buffer_disp) != 0) prenotazione(buffer_disp,buffer_posti,getpid());
                else
                {
                    printf("Disponibilita' esaurita\n");
                }
            }
            Signal_Sem(semid,MUTEX);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        wait(NULL);
    }

    kill(pid1,SIGKILL);

    shmctl(shm_id_disp,IPC_RMID,0);
    shmctl(shm_id_posti,IPC_RMID,0);
    semctl(semid,MUTEX,IPC_RMID);

    return 0;
}
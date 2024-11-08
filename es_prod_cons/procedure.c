#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"

void prenotazione(int semid, int *buff_disp, posto *p, pid_t pid)
{
    srand(time(NULL));

    Wait_Sem(semid,MUTEX);
    int tempo_sleep = rand() % 6;

    sleep(tempo_sleep);

    int num_prenotazioni = (rand() % 4) + 1;
    int temp = num_prenotazioni;
    for (int i = 0; i < num_prenotazioni; i++)
    {
        if ((*buff_disp) >= temp)
        {
            int indice = 0;
            while (indice < DIM && p[indice].stato != POSTO_LIBERO) indice++;

            p[indice].stato = POSTO_IN_AGGIORNAMENTO;
            sleep(1);
            p[indice].stato = POSTO_OCCUPATO;
            p[indice].id_cliente = pid;

            (*buff_disp)--;
            temp--;
        }
        else
        {
            perror("Disponibilita' esaurita!\n");
            exit(0);
        }
    }

    Signal_Sem(semid,MUTEX);
}
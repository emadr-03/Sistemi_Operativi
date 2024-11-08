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
    //Wait_Sem(semid,MUTEX);

    if ((*buff_disp) != 0)
    {
        int indice = 0;
        while (indice <= DIM && p[indice].stato != POSTO_LIBERO) indice++;

        p[indice].stato = POSTO_IN_AGGIORNAMENTO;
        sleep(1);
        p[indice].stato = POSTO_OCCUPATO;
        p[indice].id_cliente = pid;
        printf("Il Cliente %d ha occupato il posto di indice %d\n",pid,indice);

        (*buff_disp)--;
    }
    else
    {
        printf("Disponibilita' esaurita\n");
        exit(0);
    }

    //Signal_Sem(semid,MUTEX);
}
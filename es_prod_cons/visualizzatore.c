#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"

int main()
{
    key_t chiave_shm_posti = ftok(".",'a');
    int shm_id_posti = shmget(chiave_shm_posti,sizeof(posto)*DIM,0);

    if (shm_id_posti < 0)
    {
        perror("[Visualizzatore] Errore nell'ottenimento della memoria condivisa\n");
        exit(0);
    }

    posto *buff;
    buff = (posto *) shmat(shm_id_posti,0,0);

    while(1)
    {
        for (int i = 0; i < DIM; i++)
        {
            if (buff[i].stato == POSTO_LIBERO)
            {
                printf("Numero posto: %d\n",i);
                printf("Stato posto: LIBERO\n\n");
            }
            else if (buff[i].stato == POSTO_IN_AGGIORNAMENTO)
            {
                printf("Numero posto: %d\n",i);
                printf("Stato posto: IN AGGIORNAMENTO\n\n");
            }
            else
            {
                printf("Numero posto: %d\n",i);
                printf("Stato posto: OCCUPATO\n");
                printf("ID Cliente: %d\n\n",buff[i].id_cliente);
            }
        }
        sleep(3);
    }

    return 0;
}
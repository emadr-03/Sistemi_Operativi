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

    for (int i = 0; i < DIM; i++)
    {
        printf("\nStato posto di indice %d: %d\n\n",i,buff[i].stato);
        printf("ID Cliente: %d\n\n",buff[i].id_cliente);
        sleep(1);
    }

    return 0;
}
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

int main()
{
    key_t chiave_shm_posti = ftok(".",'a');
    int shm_id_posti = shmget(chiave_shm_posti,sizeof(posto)*DIM,0);
    posto *buff;
    buff = (posto *) shmat(shm_id_posti,0,0);

    for (int i = 0; i < DIM; i++)
    {
        printf("Stato posto di indice %d: %d\n",i,buff[i].stato);
        printf("ID Cliente al posto di indice %d: %d\n",i,buff[i].id_cliente);
        sleep(1);
    }

    return 0;
}
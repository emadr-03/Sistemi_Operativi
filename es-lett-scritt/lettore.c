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

#include "procedure.h"

int main()
{
    key_t chiave_shm = ftok(".",'a');

    int shm_buff = shmget(chiave_shm,sizeof(Buffer),0);

    if (shm_buff < 0)
    {
        printf("[Lettore] Errore nell'ottenimento del buffer\n");
        exit(0);
    }

    Buffer *buff;
    buff = (Buffer *) shmat(shm_buff,0,0);

    key_t chiave_sem = ftok(".",'b');

    int semid = semget(chiave_sem,3,0);

    if (semid < 0)
    {
        printf("[Lettore] Errore nell'ottenimento dei semafori\n");
        exit(0);
    }

    for (int i = 0; i < NUM_OPERAZIONI; i++) lettore(semid,buff);


    return 0;
}
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#include "processi.h"

int main()
{

    int vett_id;
    int buffer_id;
    int sem_id;
    int *vettore;
    int *buffer;

    vett_id = /* TBD: usare shmget() per creare un vettore di interi su 
               *      memoria condivisa, con NUM_INTERI elementi */

             shmget(IPC_PRIVATE,sizeof(int)*NUM_ELEMENTI,IPC_CREAT|0664);

    if (vett_id < 0)
    {
        perror("Impossibile creare l'array di interi condiviso");
        exit(1);
    }

    vettore = (int *) shmat(vett_id,0,0);

    if (vettore == (void *)-1)
    {
        perror("Impossibile inizializzare l'array di interi condiviso");
        exit(1);
    }

    /* Inizializza il vettore con numeri casuali tra 0 e INT_MAX */

    srand(time(NULL));

    for (int i = 0; i < NUM_ELEMENTI; i++)
    {

        vettore[i] = rand() % INT_MAX;

        //printf("%d\n", vettore[i]); // per debugging
    }

    buffer_id = /* TBD: usare shmget() per creare un buffer singolo su
                 *      memoria condivisa, con un intero */

                shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);

    if (buffer_id < 0)
    {
        perror("Impossibile creare il buffer condiviso");
        exit(1);
    }

    buffer = (int *) shmat(buffer_id,0,0);

    if (buffer == (void *)-1)
    {
        perror("Impossibile inizializzare il buffer condiviso");
        exit(1);
    }

    /* Inizializza il buffer ad INT_MAX.
     * Il valore da ricercare sarà, per definizione, minore del valore iniziale.
     */

    *buffer = INT_MAX;


    /* Inizializzazione semafori */

    sem_id = inizializza_semafori();



    /* Avvio dei processi figli */

    /* TBD: creare 10 processi figli, ognuno dei quali dovrà eseguire
     *      la funzione "figlio()". Alla funzione, passare come parametri:
     *      - il puntatore al vettore 
     *      - il puntatore al buffer singolo
     *      - l'ID del vettore di semafori
     *      - l'indice del primo elemento da elaborare
     *      - il numero di elementi da elaborare (1000)
     */


    int qta = NUM_ELEMENTI/NUM_PROCESSI;
    for (int i = 0; i < NUM_PROCESSI; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            figlio(vettore,buffer,sem_id,qta*i,qta);
            exit(0);
        }
    }


    /* Processo padre */

    padre(buffer, sem_id);


    /* Deallocazione risorse IPC */

    semctl(sem_id, 0, IPC_RMID);
    shmctl(vett_id, IPC_RMID, 0);
    shmctl(buffer_id, IPC_RMID, 0);
}


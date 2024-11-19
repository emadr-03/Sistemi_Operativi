#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "header.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 5

#define PRODUZIONI 10
#define CONSUMAZIONI 4

void produttore(VettoreProdCons * vettore);
void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer);

int main() {

    // inizializzo memorie condivise per vettore e buffer
    int shmvettore = shmget(IPC_PRIVATE,sizeof(VettoreProdCons),IPC_CREAT|0664);
    VettoreProdCons *vett = (VettoreProdCons *) shmat(shmvettore,0,0);

    int shmbuffer = shmget(IPC_PRIVATE,sizeof(BufferMutuaEsclusione),IPC_CREAT|0664);
    BufferMutuaEsclusione *buff = (BufferMutuaEsclusione *) shmat(shmbuffer,0,0);

    // inizializzo strutture

    inizializza_vettore(vett);
    inizializza_buffer(buff);

    // creo processi figli

    for(int i=0; i<NUM_PRODUTTORI; i++) {
        
        pid_t pid = fork();

        if (pid == 0)
        {
            produttore(vett);
            exit(0);
        }
    }


    for(int i=0; i<NUM_CONSUMATORI; i++) {

        pid_t pid = fork();

        if (pid == 0)
        {
            consumatore(vett,buff);
            exit(0);
        }
    }


    // attendo terminazione figli
    for (int i = 0; i < NUM_CONSUMATORI+NUM_PRODUTTORI; i++) wait(NULL);

    
    // rimuovo strutture
    rimuovi_buffer(buff);
    rimuovi_vettore(vett);
    shmctl(shmvettore,IPC_RMID,0);
    shmctl(shmbuffer,IPC_RMID,0);

    return 0;

}

void produttore(VettoreProdCons * vettore) {

    srand(getpid());
    
    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[MAIN PRODUTTORE] Produzione: %d\n", valore);

        // produzione effettiva
        produci(vettore,valore);
    }

}

void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer) {

    srand(getpid());

    for(int i=0; i<CONSUMAZIONI; i++) {

        int valore;

        // consumo e aggiornamento effettivi
        valore = consuma(vettore);

        printf("[MAIN CONSUMATORE] Consumazione: %d\n", valore);

        aggiorna(buffer,valore);
        printf("Valore aggiornato del buffer: %d\n",buffer->valore);
    }
}

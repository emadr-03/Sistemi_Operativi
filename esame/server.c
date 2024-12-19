#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "header.h"
#include <sys/wait.h>

int main() {

    /*
        TODO: recupero code e inizializzazione strutture
    */

    key_t chiave_coda_richieste = ftok(".",'a');
    key_t chiave_coda_risposte = ftok(".",'b');

    int coda_richieste = msgget(chiave_coda_richieste,0);

    if (coda_richieste < 0)
    {
        perror("[Server] Errore ottenimento coda richieste");
        exit(0);
    }

    int coda_risposte = msgget(chiave_coda_risposte,0);

    if (coda_risposte < 0)
    {
        perror("[Server] Errore ottenimento coda risposte");
        exit(0);
    }

    int shmid = shmget(IPC_PRIVATE,sizeof(MonitorLettScritt),IPC_CREAT|0664);

    if (shmid < 0)
    {
        perror("[Server] Errore creazione shared memory");
        exit(0);
    }

    MonitorLettScritt *ls = (MonitorLettScritt *) shmat(shmid,0,0);

    inizializza_monitor(ls);

    pid_t pid;

    for (int i=0; i<NUM_BAMBINI*NUM_LETTERE; i++) {
        /*
            TODO: creare figli
        */
        pid = fork();
        if (pid == 0)
        {
            Scrittore(coda_richieste,ls);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("[Server] Errore creazione figli scrittori");
            exit(0);
        }
        sleep(rand()%3 + 1);
    }

    for (int i=0; i<NUM_BAMBINI*NUM_LETTERE; i++) {
        /*
            TODO: creare figli
        */
        pid = fork();
        if (pid == 0)
        {
            Lettore(coda_risposte,ls);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("[Server] Errore creazione processi lettori");
            exit(0);
        }
        sleep(rand()%3 + 1);
    }

    /*
        TODO: attesa terminazione figli
    */

    for (int i = 0; i < 30; i++) wait(NULL);

    /*
        TODO: distruzione risorse
    */
    distruggi_monitor(ls);

    shmctl(shmid,IPC_RMID,0);

    return 0;
}
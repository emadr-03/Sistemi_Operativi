#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"
#include "time.h"

int main() {
    /*
        TODO: creazione code di messaggi 
    */

   pid_t pid;

    key_t chiave_coda_richieste = ftok(".",'a');
    key_t chiave_coda_risposte = ftok(".",'b');

    int queue_richieste = msgget(chiave_coda_richieste,IPC_CREAT|IPC_EXCL|0664);

    if (queue_richieste < 0)
    {
        perror("Errore creazione coda richieste");
        queue_richieste = msgget(chiave_coda_richieste,0);
        msgctl(queue_richieste,IPC_RMID,0);
        exit(0);
    }

    int queue_risposte = msgget(chiave_coda_risposte,IPC_CREAT|IPC_EXCL|0664);

    if (queue_risposte < 0)
    {
        perror("Errore creazione coda risposte");
        queue_risposte = msgget(chiave_coda_risposte,0);
        msgctl(queue_risposte,IPC_RMID,0);
        exit(0);
    }

    /*
        TODO: avvio del client e server in eseguibili distinti
    */

    pid = fork();
    if (pid == 0)
    {
        execl("./client","client",NULL);
        exit(0);
    }
    else if (pid < 0)
    {
        perror("Errore creazione client");
        exit(0);
    }

    pid = fork();
    if (pid == 0)
    {
        execl("./server","server",NULL);
        exit(0);
    }
    else if (pid < 0)
    {
        perror("Errore creazione server");
        exit(0);
    }

    /*
        TODO: attendere terminazione processi
    */
    wait(NULL);
    wait(NULL);

    msgctl(queue_richieste, IPC_RMID, NULL);
    msgctl(queue_risposte, IPC_RMID, NULL);

    return 0;
}
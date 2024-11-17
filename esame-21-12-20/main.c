#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_VIEWERS 3
#define NUM_UPDATERS 2


int main(){

    int status, k;
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok(".",'a');
    int key_queue_response = ftok(".",'b');
    
    // TODO aggiungere codice di inizializzazione delle code
    ds_queue_request = msgget(key_queue_request,IPC_CREAT|IPC_EXCL|0664);

    if (ds_queue_request < 0)
    {
        printf("Risorsa gia' esistente\n");
        ds_queue_request = msgget(key_queue_request,0);
        msgctl(ds_queue_request,IPC_RMID,0);
        exit(0);
    }

    ds_queue_response = msgget(key_queue_response,IPC_CREAT|IPC_EXCL|0664);

    if (ds_queue_response < 0)
    {
        printf("Risorsa gia' esistente\n");
        ds_queue_response = msgget(key_queue_response,0);
        msgctl(ds_queue_response,IPC_RMID,0);
        exit(0);
    }
    
    printf("[master] Coda ds_queue_request creata! ID: %d\n", ds_queue_request);
    printf("[master] Coda ds_queue_response creata! ID: %d\n", ds_queue_response);
    
    // TODO aggiungere codice per generare NUM_VIEWERS processi viewers, attraverso l'exec

    for (int i = 0; i < NUM_VIEWERS; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execl("./viewer","viewer",NULL);
            exit(0);
        }
    }
    

    // TODO aggiungere codice per generare NUM_UPDATERS processi updaters, attraverso l'exec

    for (int i = 0; i < NUM_UPDATERS; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execl("./updater","updater",NULL);
            exit(0);
        }
    }
    

    // TODO aggiungere codice per generare 1 processo store, attraverso l'exec

    pid_t pid = fork();
    if (pid == 0)
    {
        execl("./store","store",NULL);
        exit(0);
    }
    
    for (k=0; k<NUM_VIEWERS+NUM_UPDATERS+1; k++){
        pid = wait(&status);
        if (pid == -1)
            perror("errore");
        else
            printf ("[master] Figlio n.ro %d e\' morto con status= %d \n", pid, status);
    }

    // TODO aggiungere codice per la rimozione delle code

    msgctl(ds_queue_request,IPC_RMID,0);
    msgctl(ds_queue_response,IPC_RMID,0);
    printf("[master] code rimosse\n");
    
    return 0;
}

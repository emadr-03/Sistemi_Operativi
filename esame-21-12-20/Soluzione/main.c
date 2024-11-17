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

    pid_t pid;
    int status, k;
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok(PATH_REQ, ID_REQ);
    int key_queue_response = ftok(PATH_RESP, ID_RESP);
    
    ds_queue_request = msgget(key_queue_request, IPC_CREAT | 0644);
    ds_queue_response = msgget(key_queue_response, IPC_CREAT | 0644);
    
    printf("[master] Coda ds_queue_request creata! ID: %d\n", ds_queue_request);
    printf("[master] Coda ds_queue_response creata! ID: %d\n", ds_queue_response);
    
    for (k = 0; k < NUM_VIEWERS; k++) {
        pid = fork();
        sleep(1);
        if (pid == 0) {                //processo figlio
            execl("./viewer", "viewer", NULL);
            perror("Errore nel viewer");
            exit(-1); 
        }
    }

    for (k = 0; k < NUM_UPDATERS; k++) {
        pid = fork();
        sleep(1);
        if (pid == 0) {                //processo figlio
            execl("./updater", "updater", NULL);
            perror("Errore nel updater");
            exit(-1); 
        }
    }
    
    
    pid = fork();
    
    if (pid == 0) {                //processo figlio
        execl("./store", "store", NULL);
        perror("Errore nella execl store");
        exit(-1);
        
    }
    
    for (k=0; k<NUM_VIEWERS+NUM_UPDATERS+1; k++){
        pid = wait(&status);
        if (pid == -1)
            perror("errore");
        else
            printf ("[master] Figlio n.ro %d e\' morto con status= %d \n", pid, status);
    }

    msgctl(ds_queue_request,IPC_RMID,0);
    msgctl(ds_queue_response,IPC_RMID,0);

    printf("[master] code rimosse\n");
    
    return 0;
}

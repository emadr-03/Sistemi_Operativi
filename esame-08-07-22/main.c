#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    int pid;
    key_t msg_req_key = ftok(".",'a');
	key_t msg_res_key = ftok(".",'b');

    //TODO: INIZIALIZZAZIONE CODE
	
    int queue_req = msgget(msg_req_key,IPC_CREAT|IPC_EXCL|0664);
    if (queue_req < 0)
    {
        perror("Errore creazione coda richieste");
        queue_req = msgget(msg_req_key,0);
        msgctl(queue_req,IPC_RMID,0);
        exit(0);
    }

    int queue_res = msgget(msg_res_key,IPC_CREAT|IPC_EXCL|0664);
    if (queue_res < 0)
    {
        perror("Errore creazione coda risposte");
        queue_res = msgget(msg_res_key,0);
        msgctl(queue_res,IPC_RMID,0);
        exit(0);
    }

    pid = fork();
    if (pid==0) {
        execl("./server","./server",NULL);
        _exit(0);
    }

    pid = fork();
    if (pid==0) {
        execl("./reader","./reader",NULL);
        _exit(0);
    }

    wait(NULL);
    wait(NULL);

    // TODO: eliminazione code
    msgctl(queue_req,IPC_RMID,0);
    msgctl(queue_res,IPC_RMID,0);

    return 0;
}

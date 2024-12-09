#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


int main() {
	int pid;
	//TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok(".",'a');
	key_t msg_res_key = ftok(".",'b');

	int queue_req = msgget(msg_req_key,0);
	int queue_res = msgget(msg_res_key,0);

	int i;
	for (i=0; i < NUM_CONS; i++) {
		req msg;
		//TODO preparazione messaggio
		msg.type = MSG_RICHIESTA;
		printf("CONSUMATORE %d: invio richiesta \n",getpid());
		//TODO invio richiesta
		msgsnd(queue_req,(void *)&msg,sizeof(req)-sizeof(long),0);
		res risp;
		//TODO ricezione risposta
		msgrcv(queue_res,(void *)&risp,sizeof(res)-sizeof(long),MSG_RISPOSTA,0);
		printf("CONSUMATORE %d: letto valore: %d\n",getpid(),risp.value);
		sleep(1);
	}
	//TODO rimozione code

	return 0;
}

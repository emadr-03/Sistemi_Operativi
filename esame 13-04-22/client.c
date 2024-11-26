#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "header.h"

void updater(int queue_req) {
	int i;
	for (i=0; i < NUM_UPDATES; i++) {
		int val = rand()%100 - 40;
		req msg;
		/*TODO completare con preparazione e invio messaggio di 
		richiesta aggiornamento 'msg' con il valore 'val' */

		msg.mittente = getpid();
		msg.type = MSG_REQ;
		msg.valore = val;

		printf("UPDATER: invio richiesta aggiornamento a valore %d\n",val);

		msgsnd(queue_req,(void *)&msg,sizeof(msg)-sizeof(long),0);
	
		sleep(2);
	}
}

void reader(int queue_req, int queue_res) {
	int i;
	for (i=0; i < NUM_READS; i++) {
		req msg;
		/*TODO completare con preparazione e invio messaggio di 
		richiesta di lettura 'msg' */
		printf("READER %d: invio richiesta lettura\n",getpid());
		msg.mittente = getpid();
		msg.type = MSG_REQ;

		msgsnd(queue_req,(void *)&msg,sizeof(msg)-sizeof(long),0);
	
		/*TODO completare con ricezione risposta */

		res msg2;

		msgrcv(queue_res,(void *)&msg2,sizeof(msg2)-sizeof(long),0,0);

		int value = msg2.valore;
		printf("READER %d: letto valore: %d\n",getpid(),value);
		sleep(1);
	}
}

int main() {
	int pid;
	key_t msg_req_key = ftok(".",'a');
	key_t msg_res_key = ftok(".",'b');

	int queue_req = msgget(msg_req_key,0);
	int queue_res = msgget(msg_res_key,0);

	srand(time(NULL));
	
	// TODO creare un processo figlio che esegue la funzione 'updater'
	pid = fork();

	if (pid == 0)
	{
		updater(queue_req);
		exit(0);
	}

	// TODO creare NUM_READERS processi figlio che eseguono la funzione 'reader'

	for (int i = 0; i < NUM_READERS; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			reader(queue_req,queue_res);
			exit(0);
		}
	}

	// TODO mettere in attesa il padre dei processi figli
	for (int i = 0; i < NUM_READERS + 1; i++) wait(NULL);
	
	// TODO completare con rimozione delle code

	return 0;
}
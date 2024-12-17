#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "procedure.h"


int main() {

	int id_queue, id_shm;
	key_t chiave_shm = IPC_PRIVATE;
	key_t chiave_mbx = IPC_PRIVATE;

	id_shm = shmget(chiave_shm,sizeof(MonitorRisultati),IPC_CREAT|0664);
	if (id_shm < 0)
	{
		perror("Errore creazione shared memory");
		exit(0);
	}

	id_queue = msgget(chiave_mbx,IPC_CREAT|0664);
	if (id_queue < 0)
	{
		perror("Errore creazione coda di messaggi");
		exit(0);
	}


	MonitorRisultati * ls = (MonitorRisultati *) shmat(id_shm,0,0);

	// TODO: inizializzare struttura

	for (int i = 0; i < DIM; i++) ls->buffer[i] = 0;
	ls->testa = 0;
	ls->coda = 0;
	ls->numelementi = 0;
	init_monitor(&(ls->m),2);
	
	pid_t pid;

	int j;
	for(j=0; j < N_STAMPA ; j++) {
		
		pid = fork();
		if (pid == 0)
		{
			stampa(ls);
			exit(0);
		}
	}

	
	for(j=0; j < N_FILTRI ; j++) {
		
		pid = fork();
		if (pid == 0)
		{
			filtro(ls,id_queue);
			exit(0);
		}
	}

	for(j=0; j < N_GENERATORI ; j++) {
	
		pid = fork();
		if (pid == 0)
		{
			generazione(id_queue);
			exit(0);
		}
	}

	// TODO: attendere terminazione processi figli
	for (int i = 0; i < 9; i++) wait(NULL);

	// TODO: deallocazione strutture
	remove_monitor(&(ls->m));

	shmctl(id_shm,IPC_RMID,0);
	msgctl(id_queue,IPC_RMID,0);

	return 0;
}






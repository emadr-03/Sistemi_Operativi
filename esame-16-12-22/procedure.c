#include "procedure.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>


int stampa_risultato(MonitorRisultati * ls) {

	int risultato;

	// TODO: consumo il valore

	enter_monitor(&(ls->m));

	if (ls->numelementi == 0) wait_condition(&(ls->m),VARCOND_CONSUMATORI);

	risultato = ls->buffer[ls->coda];
	ls->coda = (ls->coda + 1) % DIM;
	ls->numelementi--;

	signal_condition(&(ls->m),VARCOND_PRODUTTORI);

	leave_monitor(&(ls->m));

	return risultato;
}


void inserisci_risultato(MonitorRisultati * ls, int valore) {

	// TODO: produco il valore
	enter_monitor(&(ls->m));

	if (ls->numelementi == DIM) wait_condition(&(ls->m),VARCOND_PRODUTTORI);

	if (valore % 2 == 0)
	{
		printf("[Produttore] Valore ricevuto: %d\n",valore);
		ls->buffer[ls->testa] = valore;
		printf("[Produttore] Valore inserito: %d\n",ls->buffer[ls->testa]);
		ls->testa = (ls->testa + 1) % DIM;
		ls->numelementi++;
	}
	else
	{
		printf("[Produttore] Valore ricevuto: %d\n",valore);
		ls->buffer[ls->testa] = 0;
		printf("[Produttore] Valore inserito: %d\n",ls->buffer[ls->testa]);
		ls->testa = (ls->testa + 1) % DIM;
		ls->numelementi++;
	}

	signal_condition(&(ls->m),VARCOND_CONSUMATORI);

	leave_monitor(&(ls->m));
}

void filtro(MonitorRisultati * ls, int coda){
	Messaggio m;
	int ret;
	int valore = 0;
	for (int i = 0 ; i < 4; i++){
	
		// TODO: filtro i valori prelevati dalla mailbox: se sono pari, li inserisco, altrimenti inserisco 0
		ret = msgrcv(coda,(void *)&m,sizeof(Messaggio)-sizeof(long),1,0);
		if (ret < 0)
		{
			perror("Errore ricezione messaggio");
			exit(0);
		}
		valore = m.val;
		inserisci_risultato(ls,valore);
	}
}

void generazione(int coda) {
	srand(time(NULL));
	int valore;
	int ret;
	Messaggio m;
	//sleep(2);
	for (int i = 0 ; i < 4; i++) {
		
		// TODO: genero dei numeri casuali tra 1 e 20 e li invio alla mailbox
		valore = (rand() % 20) + 1;
		m.type = 1;
		m.val = valore;
		ret = msgsnd(coda,(void *)&m,sizeof(Messaggio)-sizeof(long),0);
		if (ret < 0)
		{
			perror("[Generatore] Errore invio messaggio");
			exit(0);
		}
	}
}

void stampa(MonitorRisultati * ls) {
	int val;
	for (int i = 0 ; i < 4; i++) {
		
		// TODO: stampo il valore appena consumato
		val = stampa_risultato(ls);
		
		printf("[%d] Ho letto il valore: %d \n", getpid(), val);
	}
}

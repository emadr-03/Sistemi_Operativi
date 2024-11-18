#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void InizializzaMonitor(MonitorSchedulatore * s) {

	/* TBD: Inizializzare la struttura dati */
	init_monitor(&(s->m),2);
}


void RimuoviMonitor(MonitorSchedulatore * s) {

	/* TBD: Deallocare il sotto-oggetto monitor */
	remove_monitor(&(s->m));
}


void InserisciRichiesta(MonitorSchedulatore * s, richiesta * r) {

	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(s->m));


	/*
	  TBD: Verifica se la coda di richieste sia piena.
	  In caso affermativo, il processo è posto in attesa.
	*/

	if (s->contatore == DIMENSIONE_CODA) wait_condition(&(s->m),VAR_COND_UTENTE);

	printf("[%d] Produzione in testa: %d\n", getpid(), s->testa);

	s->coda_richieste[s->testa].posizione = r->posizione;
	s->coda_richieste[s->testa].processo = r->processo;

	s->testa = (s->testa + 1) % DIMENSIONE_CODA;



	/* TBD: Effettuare signal_condition() */
	s->contatore++;
	signal_condition(&(s->m),VAR_COND_SCHEDULATORE);


	/* TBD: Uscire dal monitor */
	leave_monitor(&(s->m));
}


void PrelevaRichiesta(MonitorSchedulatore * s, richiesta * r) {

	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(s->m));


	/*
	  TBD: Verifica se la coda di richieste sia vuota.
	  In caso affermativo, il processo è posto in attesa.
	*/

	if (s->contatore == 0) wait_condition(&(s->m),VAR_COND_SCHEDULATORE);


	printf("[%d] Consumazione in coda: %d\n", getpid(), s->coda);

	r->posizione = s->coda_richieste[s->coda].posizione;
	r->processo = s->coda_richieste[s->coda].processo;

	s->coda = (s->coda + 1) % DIMENSIONE_CODA;


	/* TBD: Effettuare signal_condition() */

	s->contatore--;

	signal_condition(&(s->m),VAR_COND_UTENTE);

	/* TBD: Uscire dal monitor */
	leave_monitor(&(s->m));
}



#ifndef HEADER_H
#define HEADER_H

#include "monitor_hoare.h"

#define DIM 4

#define N_GENERATORI 3
#define N_FILTRI 3
#define N_STAMPA 3

#define VARCOND_PRODUTTORI 0
#define VARCOND_CONSUMATORI 1

typedef struct {
	int buffer[DIM];
	
	// TODO: inserire i campi necessari alla gestione della coda circolare

	int testa;
	int coda;
	int numelementi;
	
	Monitor m;
} MonitorRisultati;


void inserisci_risultato(MonitorRisultati * ls, int valore);
int stampa_risultato(MonitorRisultati * ls);

typedef struct {
	
	// TODO: inserire i campi che costituiscono il messaggio da inviare
	long type;
	int val;
	
} Messaggio;

void filtro(MonitorRisultati * ls, int coda);
void generazione(int coda);
void stampa(MonitorRisultati * ls);

#endif

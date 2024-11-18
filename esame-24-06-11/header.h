#ifndef __HEADER__
#define __HEADER__

#include <sys/types.h>

#include "monitor_hoare.h"

#define TOTALE_UTENTI 5
#define TOTALE_RICHIESTE 5
#define TOTALE_POSIZIONI 20
#define DIMENSIONE_CODA 10

/* TBD: Definire delle macro per identificare le variabili condition */
#define VAR_COND_SCHEDULATORE 0
#define VAR_COND_UTENTE 1


/*
  Struttura dati che rappresenta una singola richiesta
  verso il disco
*/

typedef struct {
	int posizione;
	pid_t processo;
} richiesta;



/*
  Struttura dati che rappresenta la coda di richieste,
  in cui gli utenti collocheranno le richieste, e lo
  schedulatore preleverà le richieste.

  Contiene un array di richieste (di dimensione fissa),
  un puntatore testa e un puntatore coda, e una struttura
  dati di appoggio per il monitor.
*/

typedef struct {

	richiesta coda_richieste[DIMENSIONE_CODA];

	/* TBD: Aggiungere un sotto-oggetto Monitor e le variabili per
   *      la gestione del vettore circolare
   */
  int testa;
  int coda;
  int contatore;
  Monitor m;

} MonitorSchedulatore;


void InizializzaMonitor(MonitorSchedulatore * s);
void RimuoviMonitor(MonitorSchedulatore * s);
void InserisciRichiesta(MonitorSchedulatore * s, richiesta * r);
void PrelevaRichiesta(MonitorSchedulatore * s, richiesta * r);

void Utente(MonitorSchedulatore * s);
void Schedulatore(MonitorSchedulatore * s);


#endif /* __HEADER__ */

#ifndef _HEADER_
#define _HEADER_

#include "monitor_hoare.h"
#include "semafori.h"

#define DIM 3

// Variabili per la sincronizzazione
#define VARCOND_PRODUTTORI 0
#define VARCOND_CONSUMATORI 1
#define MUTEXC 0

// struttura per risolvere il problema prod-cons
typedef struct
{
    int buffer[DIM];
    int testa;
    int coda;
    int num_elementi;
    Monitor m;
}VettoreProdCons;


// struttura per la gestione in mutua esclusione del buffer
typedef struct
{
    int valore;
    int semid;
}BufferMutuaEsclusione;

void inizializza_vettore(VettoreProdCons * p);
void produci(VettoreProdCons * p, int valore);
int consuma(VettoreProdCons * p);
void rimuovi_vettore(VettoreProdCons * p);

void inizializza_buffer(BufferMutuaEsclusione * p);
void aggiorna(BufferMutuaEsclusione * p, int valore);
void rimuovi_buffer(BufferMutuaEsclusione * p);


#endif

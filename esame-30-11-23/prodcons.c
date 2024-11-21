#include <stdio.h>

#include "prodcons.h"


void init_monitor(MonitorProdCons * p) {

    /* TBD: Inizializzare l'oggetto monitor */

    p->conteggio_consumazioni = 0;
    for (int i = 0; i < DIM; i++) p->buffer[i] = 0;
    p->coda = 0;
    p->testa = 0;
    p->num_elementi = 0;
    pthread_mutex_init(&(p->mutex),NULL);
    pthread_cond_init(&(p->cond_consumatori),NULL);
    pthread_cond_init(&(p->cond_produttori),NULL);
    pthread_cond_init(&(p->cond_consumazioni),NULL);
}

void produzione(MonitorProdCons * p, int val) {

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */

    pthread_mutex_lock(&(p->mutex));

    while (p->num_elementi == DIM) pthread_cond_wait(&(p->cond_produttori),&(p->mutex));

    p->buffer[p->testa] = val;
    printf("[PRODUTTORE] Valore prodotto: %d\n",p->buffer[p->testa]);
    p->testa = (p->testa + 1) % DIM;
    p->num_elementi++;

    pthread_cond_signal(&(p->cond_consumatori));

    pthread_mutex_unlock(&(p->mutex));
}

int consumazione(MonitorProdCons * p) {

    int val;

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */

    pthread_mutex_lock(&(p->mutex));

    while (p->num_elementi == 0) pthread_cond_wait(&(p->cond_consumatori),&(p->mutex));

    val = p->buffer[p->coda];
    printf("[CONSUMATORE] Valore consumato: %d\n",p->buffer[p->coda]);
    p->coda = (p->coda + 1) % DIM;
    p->num_elementi--;

    p->conteggio_consumazioni++;

    pthread_cond_signal(&(p->cond_consumazioni));
    pthread_cond_signal(&(p->cond_produttori));

    pthread_mutex_unlock(&(p->mutex));

    return val;
}

int attendi_consumazioni(MonitorProdCons * p, int soglia) {

    int n;

    /* TBD: Sospendere il thread finché il "conteggio_consumazioni" non sia diventato
            maggiore o uguale a "soglia"
     */

    pthread_mutex_lock(&(p->mutex));

    while (p->conteggio_consumazioni < soglia) pthread_cond_wait(&(p->cond_consumazioni),&(p->mutex));

    n = p->conteggio_consumazioni;

    p->conteggio_consumazioni = 0;

    pthread_mutex_unlock(&(p->mutex));
    
    return n;
}

void remove_monitor(MonitorProdCons * p) {

    /* TBD: Disattivare le variabili per la sincronizzazione */

    pthread_mutex_destroy(&(p->mutex));
    pthread_cond_destroy(&(p->cond_consumatori));
    pthread_cond_destroy(&(p->cond_consumazioni));
    pthread_cond_destroy(&(p->cond_produttori));
}
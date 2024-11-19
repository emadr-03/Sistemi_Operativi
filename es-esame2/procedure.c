#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

void inizializza_vettore(VettoreProdCons * p) {

   for (int i = 0; i < DIM; i++) p->buffer[i] = 0;
   p->coda = 0;
   p->testa = 0;
   p->num_elementi = 0;
   init_monitor(&(p->m),2);
}

void produci(VettoreProdCons * p, int valore) {

    // produzione con meccanismo di coda circolare
    enter_monitor(&(p->m));

    if (p->num_elementi == DIM) wait_condition(&(p->m),VARCOND_PRODUTTORI);

    p->buffer[p->testa] = valore;
    p->testa = (p->testa + 1) % DIM;
    p->num_elementi++;
    
    signal_condition(&(p->m),VARCOND_CONSUMATORI);

    leave_monitor(&(p->m));
}

int consuma(VettoreProdCons * p) {

    // consumo con meccanismo di coda circolare
    int valore;
    enter_monitor(&(p->m));

    if (p->num_elementi == 0) wait_condition(&(p->m),VARCOND_CONSUMATORI);

    valore = p->buffer[p->coda];
    p->coda = (p->coda + 1) % DIM;

    p->num_elementi--;

    signal_condition(&(p->m),VARCOND_PRODUTTORI);

    leave_monitor(&(p->m));
    return valore;
}

void rimuovi_vettore(VettoreProdCons * p) {

    // 
    remove_monitor(&(p->m));
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

   // inizializzazione semaforo per aggiornare il buffer in mutua esclusione
    p->valore = 0;
    p->semid = semget(IPC_PRIVATE,1,IPC_CREAT|0664);
    semctl(p->semid,MUTEXC,SETVAL,1);
}

void aggiorna(BufferMutuaEsclusione * p, int valore) {

    // aggiorno valore in mutua esclusione
    Wait_Sem(p->semid,MUTEXC);
    p->valore += valore;
    Signal_Sem(p->semid,MUTEXC);
}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    //
    semctl(p->semid,MUTEXC,IPC_RMID);
}



#include <stdio.h>

#include "prodcons.h"


void init_monitor_circolare(MonitorVettoreCircolare * p) {

    /* TBD: Inizializzare il monitor */

    init_monitor(&(p->m),2);

    for (int i = 0; i < DIM; i++) p->buffer[i] = 0;
    p->testa = 0;
    p->coda = 0;
    p->numelementi = 0;
}

void produzione_circolare(MonitorVettoreCircolare * p, int val) {

    /* TBD: Completare la produzione nel buffer circolare con sincronizzazione */

    enter_monitor(&(p->m));

    if (p->numelementi == DIM) wait_condition(&(p->m),VARCOND_PRODUTTORI);

    p->buffer[p->testa] = val;
    printf("[PRODUTTORE] Prodotto: %d\n",p->buffer[p->testa]);
    p->testa = (p->testa + 1) % DIM;

    p->numelementi++;

    signal_condition(&(p->m),VARCOND_CONSUMATORI);

    leave_monitor(&(p->m));
}

int consumazione_circolare(MonitorVettoreCircolare * p) {

    /* TBD: Completare la consumazione dal buffer circolare con sincronizzazione */

    enter_monitor(&(p->m));

    if (p->numelementi == 0) wait_condition(&(p->m),VARCOND_CONSUMATORI);

    int val;

    val = p->buffer[p->coda];
    printf("[CONSUMATORE] Consumato: %d\n",val);
    p->coda = (p->coda + 1) % DIM;

    p->numelementi--;

    signal_condition(&(p->m),VARCOND_PRODUTTORI);

    leave_monitor(&(p->m));

    return val;
}

void remove_monitor_circolare(MonitorVettoreCircolare * p) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&(p->m));
}



void init_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Inizializzare il monitor */
    init_monitor(&(b->m),2);
    b->buffer = 0;
    b->bufferlibero = 1;
}

void produzione_buffer_singolo(MonitorBufferSingolo * b, int val) {

    /* TBD: Completare la produzione sul buffer singolo con sincronizzazione */
    
    enter_monitor(&(b->m));

    if (b->bufferlibero == 0) wait_condition(&(b->m),VARCOND_PRODUTTORI);

    b->buffer = val;

    b->bufferlibero = 0;

    signal_condition(&(b->m),VARCOND_CONSUMATORI);

    leave_monitor(&(b->m));
}

int consumazione_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Completare la consumazione dal buffer singolo con sincronizzazione */

    enter_monitor(&(b->m));

    if (b->bufferlibero == 1) wait_condition(&(b->m),VARCOND_CONSUMATORI);

    int val;

    val = b->buffer;

    b->bufferlibero = 1;

    signal_condition(&(b->m),VARCOND_PRODUTTORI);

    leave_monitor(&(b->m));

    return val;
}

void remove_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&(b->m));
}

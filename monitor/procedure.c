#include <stdio.h>
#include <time.h>
#include "procedure.h"

void produttore(prodcons *p)
{
    int valore;
    srand(time(NULL)*getpid());
    entermonitor(&(p->m));

    if (p->buffer_libero == 0) Wait_Cond(&(p->m),VARCOND_PRODUTTORE);

    valore = rand() % 51;

    p->buffer = valore;
    p->buffer_libero = 0;

    sleep(2);
    printf("[Produttore] Valore prodotto: %d\n",p->buffer);

    Signal_Cond(&(p->m),VARCOND_CONSUMATORE);
    leavemonitor(&(p->m));
}


void consumatore(prodcons *p)
{
    entermonitor(&(p->m));

    if (p->buffer_libero == 1) Wait_Cond(&(p->m),VARCOND_CONSUMATORE);

    sleep(2);
    printf("[Consumatore] Valore consumato: %d\n",p->buffer);
    p->buffer_libero = 1;

    Signal_Cond(&(p->m),VARCOND_PRODUTTORE);
    leavemonitor(&(p->m));
}
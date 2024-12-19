#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include "monitor_signal_continue.h"

#define NUM_BAMBINI 3
#define NUM_LETTERE 5

/*NOTA

    Il programma sembra bloccarsi (almeno sul mio pc) dopo che lo scrittore effettua le prime tre stampe, ma in realtà
    riprende da solo dopo qualche secondo e termina normalmente
    ciò è dovuto alle sleep inserite lato server quando vengono creati i processi figli
    ho preferito lasciarle per far notare meglio l'esecuzione del programma

*/

/*
    TODO: completare le strutture necessarie allo scambio di messaggi e alla risoluzione del problema lett-scritt con monitor
*/

#define VARCOND_LETTORI 0
#define VARCOND_SCRITTORI 1

typedef struct {

    long type;
    int val;

} MessaggioRichiesta;

typedef struct {

    long type;

} MessaggioRisposta;

typedef struct {

    int totale_regali;

    int numlettori;
    int numscrittori;

    Monitor m;
} MonitorLettScritt;

// Funzioni per il monitor lettori-scrittori
void inizializza_monitor(MonitorLettScritt *ls);
void distruggi_monitor(MonitorLettScritt *ls);
void inizio_lettura(MonitorLettScritt *ls);
void fine_lettura(MonitorLettScritt *ls);
void inizio_scrittura(MonitorLettScritt *ls);
void fine_scrittura(MonitorLettScritt *ls);

void Scrittore(int codareq, MonitorLettScritt *ls);
void Lettore(int codaresp, MonitorLettScritt *ls);

#endif
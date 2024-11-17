#ifndef HEADERM_H
#define HEADERM_H

#include "header.h"

#define SCORTA_INIZIALE 20

typedef struct {
    //Var sincronizzazione
    int num_lettori[MAX_PRODOTTI];
    int synch_sem;
    int mutexl_sem;
    //Var condivise
    int scorta[MAX_PRODOTTI];
}Magazzino;


void init_magazzino(Magazzino *magazzino);
void destroy_magazzino(Magazzino *magazzino);

void lettore(Magazzino *magazzino);
void scrittore(Magazzino *magazzino);

void InizioLettura(Magazzino *magazzino, int id_prodotto);
void FineLettura(Magazzino *magazzino, int id_prodotto);
void InizioScrittura(Magazzino *magazzino, int id_prodotto);
void FineScrittura(Magazzino *magazzino, int id_prodotto);



#endif

#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>



typedef struct {

	int molo;
	int id_nave;

	/* TODO: Aggiungere variabili per la sincronizzazione */

	pthread_mutex_t mutex;
	pthread_cond_t cond_lettori;
	pthread_cond_t cond_scrittori;

	int num_scrittori_in_attesa;
	int numscrittori;
	int numlettori;
}monitor;

void inizializza(monitor * m);
void rimuovi (monitor * m);
void scrivi_molo(monitor * m, int molo);
int leggi_molo(monitor * m);



#endif

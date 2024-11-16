#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "monitor_hoare.h"

#define NUM_UTENTI 10
#define NUM_CONDITIONS 2

#define SYNCHL 0
#define SYNCHS 1

typedef struct {
	int temperatura;
	unsigned int umidita;
	unsigned short pioggia;
} meteo;

typedef struct {
	meteo meteo;
	
	int numlettori;
	int numscrittori;

	Monitor m;
} Buffer;


void Servizio(Buffer * buf);
void Utente(Buffer * buf);
void InizioLettura(Buffer *buf);
void FineLettura(Buffer *buf);
void InizioScrittura(Buffer *buf);
void FineScrittura(Buffer *buf);

#endif

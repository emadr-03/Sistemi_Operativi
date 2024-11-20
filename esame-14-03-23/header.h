#ifndef HEADER_H
#define HEADER_H

#define NUM_OPERAZIONI 5
#define NUM_LETTORI 4
#define MUTEXL 0
#define SYNCH 1

typedef struct {
	int id_volo;
	int gate;
	//TODO: completare con le variabili per la sincronizzazione
	int numlettori;
	int id_sem;
	
} Info_Volo;

typedef struct {
	long volo;
	int gate;	
} msg;

void Wait_Sem(int, int );
void Signal_Sem (int, int );
void Lettore(Info_Volo* volo,int coda);
void Scrittore(Info_Volo* volo1,Info_Volo* volo2);

#endif

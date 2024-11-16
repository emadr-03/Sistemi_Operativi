#ifndef PROCEDURE_H
#define PROCEDURE_H

#define MUTEXL 0
#define SYNCH 1
#define MAX_DIM 15
#define NUM_ANALIZZATORI 2

typedef struct
{
    int numlettori;
    char stringa[MAX_DIM];
}Buffer;

void iniziolettura(Buffer *p,int semid);
void finelettura(Buffer *p,int semid);
void scrittore(Buffer *p,int *len,int semid);
void lettore_elaboratore(Buffer *p,int *len,int semid);
void lettore_analizzatore(Buffer *p,int *len,int semid);

#endif
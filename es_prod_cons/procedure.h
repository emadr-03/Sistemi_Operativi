#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>



#define POSTO_LIBERO 0
#define POSTO_IN_AGGIORNAMENTO 2
#define POSTO_OCCUPATO 1
#define MUTEX 0
#define DIM 80
#define NUM_CLIENTI 50

typedef struct {
    unsigned int id_cliente;
    unsigned int stato; 
} posto;

void prenotazione(int semid, int *buff_disp, posto *p, pid_t pid);

#endif
#ifndef SEMAFORI_H
#define SEMAFORI_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void Wait_Sem(int semid,int numsem);
void Signal_Sem(int semid,int numsem);

#endif
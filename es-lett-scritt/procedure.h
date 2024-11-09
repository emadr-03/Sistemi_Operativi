#ifndef PROCEDURE_H
#define PROCEDURE_H

#define MUTEX 0
#define SYNCH 1
#define MUTEXS 2
#define NUM_OPERAZIONI 5
#define NUM_LETTORI 2
#define NUM_SCRITTORI 2

typedef struct
{
    int num_lettori;
    int num_scrittori;
    int val1;
    int val2;
}Buffer;

void iniziolettura(int semid,Buffer *p);
void finelettura(int semid,Buffer *p);
void lettore(int semid,Buffer *p);
void inizioscrittura(int semid, Buffer *p);
void finescrittura(int semid, Buffer *p);
void scrittore(int semid,Buffer *p);

#endif
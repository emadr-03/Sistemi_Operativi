#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "monitor.h"

#define VARCOND_PRODUTTORE 0
#define VARCOND_CONSUMATORE 1
#define NUM_CONSUMATORI 5
#define NUM_PRODUTTORI 5

typedef struct
{
    int buffer;
    int buffer_libero; //0 occupato, 1 libero

    Monitor m;
}prodcons;

void produttore(prodcons *p);
void consumatore(prodcons *p);

#endif
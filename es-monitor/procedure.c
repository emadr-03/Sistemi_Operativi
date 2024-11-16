#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "procedure.h"

void InizioLettura(Buffer * buf){
	
	enter_monitor(&(buf->m));

	if (buf->numscrittori > 0) wait_condition(&(buf->m),SYNCHL);

	buf->numlettori++;

	signal_condition(&(buf->m),SYNCHL);

	leave_monitor(&(buf->m));
}

void FineLettura(Buffer * buf) {

	enter_monitor(&(buf->m));

	buf->numlettori--;

	if(buf->numlettori == 0) signal_condition(&(buf->m),SYNCHS);

	leave_monitor(&(buf->m));
}



//Procedure di inizio e fine scrittura

void InizioScrittura(Buffer*buf){

	enter_monitor(&(buf->m));

	if (buf->numlettori > 0 || buf->numscrittori > 0) wait_condition(&(buf->m),SYNCHS);

	buf->numscrittori++;

	leave_monitor(&(buf->m));
}

void FineScrittura(Buffer*buf){

	enter_monitor(&(buf->m));

	buf->numscrittori--;

	if (queue_condition(&(buf->m),SYNCHS)) signal_condition(&(buf->m),SYNCHS);
	else signal_condition(&(buf->m),SYNCHL);

	leave_monitor(&(buf->m));
}



void Servizio(Buffer * buf){

	srand(time(0));

	int i;
	for(i=0; i<20; i++) {

		InizioScrittura(buf);

		buf->meteo.temperatura = (rand()%101)-50;
		buf->meteo.umidita = rand()%101;
		buf->meteo.pioggia = rand()%2;

		printf("<%d> scrittura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), buf->meteo.temperatura, buf->meteo.umidita, (buf->meteo.pioggia ? "si" : "no") );

		sleep(2);

		FineScrittura(buf);
	}

}

void Utente(Buffer * buf) {

	int i;
	for(i=0; i<10; i++) {
		
		InizioLettura(buf);

		printf("<%d> lettura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), buf->meteo.temperatura, buf->meteo.umidita, (buf->meteo.pioggia ? "si" : "no") );

		sleep(1);

		FineLettura(buf);
	}

}



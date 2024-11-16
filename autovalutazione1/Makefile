all: main generatore analizzatore elaboratore

main: main.o procedure.o semafori.o
	gcc -o main main.o procedure.o semafori.o

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.c procedure.h semafori.h
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

generatore: generatore.o procedure.o semafori.o
	gcc -o generatore generatore.o procedure.o semafori.o

generatore.o: generatore.c procedure.h
	gcc -c generatore.c

analizzatore: analizzatore.o procedure.o semafori.o
	gcc -o analizzatore analizzatore.o procedure.o semafori.o

analizzatore.o: analizzatore.c procedure.h
	gcc -c analizzatore.c

elaboratore: elaboratore.o procedure.o semafori.o
	gcc -o elaboratore elaboratore.o procedure.o semafori.o

elaboratore.o: elaboratore.c procedure.h
	gcc -c elaboratore.c
	
clean:
	rm -f *.o
	rm -f main
	rm -f analizzatore
	rm -f elaboratore
	rm -f generatore
all: main visualizzatore

main: main.o semafori.o procedure.o
	gcc -o main main.o semafori.o procedure.o

main.o: main.c semafori.h procedure.h
	gcc -c main.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

visualizzatore: visualizzatore.o procedure.o
	gcc -o visualizzatore visualizzatore.o procedure.o

visualizzatore.o: visualizzatore.c procedure.h
	gcc -c visualizzatore.c

clean:
	rm -f *.o
	rm -f main
	rm -f visualizzatore
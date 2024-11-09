all: main lettore scrittore

main: main.o procedure.o semafori.o
	gcc -o main main.o procedure.o semafori.o

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.c procedure.h semafori.h
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

lettore: lettore.o procedure.o semafori.o
	gcc -o lettore lettore.o procedure.o semafori.o

lettore.o: lettore.c procedure.h
	gcc -c lettore.c

scrittore: scrittore.o procedure.o semafori.o
	gcc -o scrittore scrittore.o procedure.o semafori.o

scrittore.o: scrittore.c procedure.h
	gcc -c scrittore.c

clean:
	rm -f *.o
	rm -f main
	rm -f lettore
	rm -f scrittore
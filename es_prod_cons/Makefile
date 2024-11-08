all: main visualizzatore

main: main.o procedure.o semafori.o
	gcc -o main main.o procedure.o semafori.o

main.o: main.c
	gcc -c main.c

procedure.o: procedure.c 
	gcc -c procedure.c 

semafori.o: semafori.c 
	gcc -c semafori.c 

visualizzatore: visualizzatore.o
	gcc -o visualizzatore visualizzatore.o

visualizzatore.o: visualizzatore.c
	gcc -c visualizzatore.c

clean:
	rm -f *.o
	rm -f main
	rm -f visualizzatore
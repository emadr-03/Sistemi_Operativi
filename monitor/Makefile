all: main

main: main.o procedure.o monitor.o
	gcc -o main main.o procedure.o monitor.o

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

monitor.o: monitor.c monitor.h
	gcc -c monitor.c

clean:
	rm -f *.o
	rm -f main
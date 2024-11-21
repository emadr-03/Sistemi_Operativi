LFLAGS=-pthread

all: main

main: main.o procedure.o
	gcc $(LFLAGS) -o main main.o procedure.o

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f main
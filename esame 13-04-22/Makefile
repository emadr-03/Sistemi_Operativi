all: main server client

main: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c main.c

server: server.o
	gcc -o server server.o

server.o: server.c header.h
	gcc -c server.c

client: client.o
	gcc -o client client.o

client.o: client.c header.h
	gcc -c client.c

clean:
	rm -f *.o
	rm -f main
	rm -f server
	rm -f client
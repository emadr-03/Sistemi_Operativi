all: consumatore server main

consumatore: consumatore.c header.h
	gcc -o consumatore consumatore.c

server: server.c header.h
	gcc -o server server.c -pthread

main: main.c
	gcc -o main main.c

clean:
	rm consumatore server main

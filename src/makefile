CC=gcc -g
CFLAGS=-std=c99 -I../header -I../src -Wall -Wextra


all: db

db: server.o database.o parser.o
	$(CC) -o db server.o database.o parser.o $(CFLAGS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -o server.o

database.o: database.c
	$(CC) $(CFLAGS) -c database.c -o database.o

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o


clean:
	rm -rf *o *~

cleanall:
	rm -rf *o *~ db

test:
	make
	make clean

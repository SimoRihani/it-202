CC=gcc -g
CFLAGS=-std=c99 -I../header -I../src -Wall -Wextra


all: tests

tests: main.o test_database.o database.o
	$(CC) -o tests main.o test_database.o database.o $(CFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o


test_database.o: test_database.c
	$(CC) $(CFLAGS) -c test_database.c -o test_database.o

database.o: ../src/database.c
	$(CC) $(CFLAGS) -c ../src/database.c	-o database.o

clean:
	rm -rf *o *~

cleanall:
	rm -rf *o *~ ../src/*o ../src/*~ ../header/*~ tests

test:
	make
	make clean
	./tests

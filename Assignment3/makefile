CC = gcc
CFLAGS = -Wall -std=c99

A3: main.o linkedList.o parser.o
	$(CC) $(CFLAGS) -o $@ main.o linkedList.o parser.o

main.o: main.c parser.o linkedList.o
	$(CC) $(CFLAGS) -c main.c -o $@

linkedList.o: linkedList.c linkedList.h parser.o
	$(CC) $(CFLAGS) -c linkedList.c -o $@

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o $@

clean:
	rm *.o




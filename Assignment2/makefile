CC = gcc
CFLAGS = -Wpedantic -std=c99 -Wall

simcpu: main.o parser.o schedule.o heap.o
	$(CC) $(CFLAGS) -o $@ main.o parser.o schedule.o heap.o

main.o: main.c parser.o schedule.o
	$(CC) $(CFLAGS) -c main.c -o $@

schedule.o: schedule.c schedule.h parser.o heap.o
	$(CC) $(CFLAGS) -c schedule.c -o $@

heap.o: heap.c heap.h parser.o
	$(CC) $(CFLAGS) -c heap.c -o $@

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o $@

clean:
	rm *.o
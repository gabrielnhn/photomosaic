# Makefile
CC = gcc
CFLAGS = -Wall
LDLIBS = -lm

all: mosaico

mosaico: main.o functions.o
	$(CC) $(CFLAGS) main.o functions.o -o mosaic  $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c  $(LDLIBS)

test: functions.o
	$(CC) $(CFLAGS) test.c functions.o  $(LDLIBS)

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

purge: clean
	rm -f mosaico
	
clean:
	rm -f *.o

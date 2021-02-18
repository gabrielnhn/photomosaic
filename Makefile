# Makefile
CC = gcc
CFLAGS = -Wall
LDLIBS = -lm

all: a.out

a.out: main.o functions.o
	$(CC) $(CFLAGS) main.o functions.o -o a.out  $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c  $(LDLIBS)

test: functions.o
	$(CC) $(CFLAGS) test.c functions.o  $(LDLIBS)

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

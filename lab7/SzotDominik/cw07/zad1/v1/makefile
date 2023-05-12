SHELL := /bin/bash
CC=gcc
CFLAGS=-Wall -O3
OTHER=utils.c queue.c

all: 
	make simulation

simulation: client barber
	$(CC) $(CFLAGS) $(SOURCES) simulation.c $(OTHER) -o simulation.exe

client:
	$(CC) $(CFLAGS) $(SOURCES) client.c $(OTHER) -o client.exe

barber:
	$(CC) $(CFLAGS) $(SOURCES) barber.c $(OTHER) -o barber.exe

clean:
	rm -rf barber.exe client.exe simulation.exe
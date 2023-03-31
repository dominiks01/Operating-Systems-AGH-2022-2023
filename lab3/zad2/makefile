CC = gcc -Wall 

all: test

compile: 
	$(CC) zadanie2.c -o ./zadanie2.exe
	
test:
	make compile
	./zadanie2.exe .
	make clean
	
clean:
	rm -f *.o 2> /dev/null
	rm -f *.exe 2> /dev/null
	rm -f *.out 2> /dev/null
	rm -f *.in 2> /dev/null


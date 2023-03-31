CC = gcc -Wall 

all: test

compile: 
	$(CC) zadanie1.c -o ./zadanie1.exe
	
test:
	make compile
	./zadanie1.exe 5
	make clean
	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null


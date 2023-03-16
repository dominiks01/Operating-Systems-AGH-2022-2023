CC = gcc -Wall 

all: test

compile:
	$(CC) zad4.c -o zad4.exe 
	chmod 777 ./zad4.exe 

test:
	make compile
	./zad4.exe .

	bash -c "find . | xargs wc --bytes";
	
clean:
	rm -f *.o 2> /dev/null
	rm -f *.exe 2> /dev/null
	rm -f *.out 2> /dev/null

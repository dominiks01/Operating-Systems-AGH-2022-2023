CC = gcc -Wall 

all: test

compile:
	$(CC) countSize.c -o countSize.exe 
	chmod 777 ./countSize.exe 

test:
	make compile
	./countSize.exe test.txt
	
	bash -c "wc --bytes *";


clean:
	rm -f *.o 2> /dev/null
	rm -f *.exe 2> /dev/null
	rm -f *.out 2> /dev/null

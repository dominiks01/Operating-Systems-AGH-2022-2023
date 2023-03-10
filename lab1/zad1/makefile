CC = gcc -Wall -ggdb

ll: static shared clean

static: 
	$(CC) -c memoryBlock.c
	ar rcs memoryBlock.a memoryBlock.o

shared: 
	$(CC) -c -fPIC memoryBlock.c 
	$(CC) -shared -fPIC -o memoryBlock.so memoryBlock.o

clean:
	rm -f *.o 2> /dev/null
	rm -f *.a 2> /dev/null
	rm -f *.so 2> /dev/null

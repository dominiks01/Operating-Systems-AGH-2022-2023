CC = gcc -Wall -ggdb

ll: dl_open clean

static: 
	$(CC) -c memoryBlock.c
	ar rcs memoryBlock.a memoryBlock.o

shared: 
	$(CC) -c -fPIC memoryBlock.c 
	$(CC) -shared -fPIC -o memoryBlock.so memoryBlock.o

clean:
	rm -f *.o 2> /dev/null
	rm -f *.so 2> /dev/null
	rm -f *.a 2> /dev/null
	rm -f *.exe 2> /dev/null

static_compile: static
	$(CC) -o main.exe zadanie2.c -L. memoryBlock.a -I .

shared_compile:  shared
	$(CC)  zadanie2.c -o main.exe  -L . memoryBlock.so
	sudo mv memoryBlock.so /usr/lib

dl_open: shared
	$(CC) zadanie2_dl.c -o main.exe -ldl


CC = gcc -Wall -ggdb
ll: test

static: 
	$(CC) -c memoryBlock.c
	ar rcs memoryBlock.a memoryBlock.o

shared: 
	$(CC) -c -fPIC memoryBlock.c 
	$(CC) -shared -fPIC -o memoryBlock.so memoryBlock.o

static_compile: static
	$(CC) -o main_static.exe zadanie2.c -L. memoryBlock.a -I .

shared_compile:  shared
	$(CC)  zadanie2.c -o main_shared.exe  -L . memoryBlock.so
	sudo mv memoryBlock.so /usr/lib	

dl_open: shared
	$(CC) zadanie2_dl.c -o main_dynamic.exe -ldl

test: 
	make static_compile
	make shared_compile
	make dl_open

	make innit_test

	./main_static.exe < main.in > ./raport3a/result_static.txt
	./main_shared.exe < main.in > ./raport3a/result_shared.txt
	./main_dynamic.exe < main.in > ./raport3a/result_dynamic.txt

innit_test:
	echo "init 10" > main.in

	echo "count memoryBlock.c" >> main.in
	echo "count memoryBlock.h" >> main.in
	echo "count zadanie2.c" >> main.in
	echo "count test.txt" >> main.in
	echo "count makefile" >> main.in

	echo "show 1" >> main.in
	echo "show 2" >> main.in
	echo "show 0" >> main.in
	echo "show 3" >> main.in
	echo "show 4" >> main.in

	echo "delete 1" >> main.in
	echo "delete 2" >> main.in
	echo "delete 0" >> main.in
	echo "delete 3" >> main.in
	echo "delete 4" >> main.in

	echo "destroy"

clean:
	rm -f *.$(shell ls | grep -e *.o -e *.so -e *.a -e *.in) 2> /dev/null
	rm -f main.in 2> /dev/null
	rm -f *.exe 2> /dev/null
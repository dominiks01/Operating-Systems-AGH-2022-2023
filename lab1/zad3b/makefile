CC = gcc -Wall -ggdb -O

CCO = gcc -Wall -ggdb -O
CC3 = gcc -Wall -ggdb -O3
CCs = gcc -Wall -ggdb -Os

FLAG = 0

all:  test

static: 
	$(CC) -c memoryBlock.c
	ar rcs memoryBlock.a memoryBlock.o

shared: 
	$(CC) -c -fPIC memoryBlock.c 
	$(CC) -shared -fPIC -o memoryBlock.so memoryBlock.o

static_compile: static
	$(CC) -o main_static_$(FLAG).exe zadanie2.c -L. memoryBlock.a -I .

shared_compile:  shared
	$(CC)  zadanie2.c -o main_shared_$(FLAG).exe  -L . memoryBlock.so
	sudo mv memoryBlock.so /usr/lib

dl_open:
	make shared
	$(CC) zadanie2_dl.c -o main_dynamic_$(FLAG).exe -ldl

test:
	make innit_test
	make compile_all
	make execute_tests

	@$(eval CC := gcc -Wall -ggdb -O)
	@$(eval FLAG := 03)

	make innit_test
	make compile_all
	make execute_tests 

	@$(eval CC := gcc -Wall -ggdb -Os)
	@$(eval FLAG := 0s)

	make innit_test
	make compile_all
	make execute_tests 

execute_tests:
	echo "main_static_$(FLAG).exe"

	./main_static_$(FLAG).exe < main.in > result_static_$(FLAG).txt
	./main_shared_$(FLAG).exe < main.in > result_shared_$(FLAG).txt
	./main_dynamic_$(FLAG).exe < main.in > result_dynamic_$(FLAG).txt

compile_all:
	make static_compile
	make shared_compile
	make dl_open

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

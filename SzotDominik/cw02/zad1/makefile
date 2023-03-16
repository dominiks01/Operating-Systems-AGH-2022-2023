CC = gcc -Wall 

all: test

systems_functions_compile: 
	$(CC) zamien.c -o system_function.exe 
	chmod 777 ./system_function.exe 

library_functions_compile:
	$(CC) zamien2.c -o library_function.exe 
	chmod 777 ./library_function.exe 

test:
	make systems_functions_compile
	./system_function.exe a x alaMaKota.txt alaMaKota2.txt >> pomiar_zad_1.txt
	
	make library_functions_compile
	./library_function.exe a x alaMaKota.txt alaMaKota2.txt >> pomiar_zad_1.txt

	bash -c "tr a x < alaMaKota.txt > alaMaKota2.txt ; diff -s alaMaKota2.txt alaMaKota.txt"

clean:
	rm -f *.o 2> /dev/null
	rm -f *.exe 2> /dev/null
	rm -f *.out 2> /dev/null



CC = gcc -Wall 

all: byte_read_variant block_read_variant

byte_read_variant: 
	$(CC) reverse.c -o byte_reverse.exe 
	chmod 777 ./byte_reverse.exe 

block_read_variant:
	$(CC) reverse2.c -o block_reverse.exe 
	chmod 777 ./block_reverse.exe 

test:
	make byte_read_variant
	./byte_reverse.exe alaMaKota.txt tmp.txt >> pomiar_zad_2.txt
	
	make block_read_variant
	./block_reverse.exe  alaMaKota.txt tmp2.txt >> pomiar_zad_2.txt

	diff -s tmp.txt tmp2.txt >> pomiar_zad_2.txt
	
clean:
	rm -f *.o 2> /dev/null
	rm -f *.exe 2> /dev/null
	rm -f *.out 2> /dev/null

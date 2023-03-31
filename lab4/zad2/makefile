CC = gcc -Wall 
FILE = zadanie2.c
CFLAGS = -Wall

all: test

compile: $(FILE) 
	$(CC) $(CFLAGS) $< -o  $(FILE:.c=.exe)
	
test: clean compile
	# make compile

	echo "" > raport.txt
	./$(FILE:.c=.exe) >> raport.txt
	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null

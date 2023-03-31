CC = gcc -Wall 
FILE = zadanie1.c
CFLAGS = -Wall

all: test

compile: $(FILE) 
	$(CC) $(CFLAGS) $< -o  $(FILE:.c=.exe)
	
test: clean compile
	# make compile

	echo > fork.txt
	echo > exec.txt

	echo "Ignore:" >> fork.txt
	./$(FILE:.c=.exe) ignore fork >> fork.txt
	echo >> fork.txt

	echo "Handler:" >> fork.txt
	./$(FILE:.c=.exe) handler fork >> fork.txt
	echo >> fork.txt

	echo "Mask:" >> fork.txt
	./$(FILE:.c=.exe) mask fork >> fork.txt
	echo >> fork.txt

	echo "Pending:" >> fork.txt
	./$(FILE:.c=.exe) pending fork >> fork.txt
	echo >> fork.txt

	# EXEC FUNCTION

	echo "Ignore:" >> exec.txt
	./$(FILE:.c=.exe) ignore exec >> exec.txt
	echo >> exec.txt

	echo "Mask:" >> exec.txt
	./$(FILE:.c=.exe) mask exec >> exec.txt
	echo >> exec.txt

	echo "Pending:" >> exec.txt
	./$(FILE:.c=.exe) pending exec >> exec.txt
	echo >> exec.txt

	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null

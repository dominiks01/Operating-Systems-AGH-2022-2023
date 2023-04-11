CC = gcc -Wall 
FILE = program.c
CFLAGS = -Wall

all: test

compile: $(FILE) 
	$(CC) $(CFLAGS) $< -o  $(FILE:.c=.exe)
	
test: clean compile
	# make compile

	echo "[NO] [NUMBER OF CHILDREN] [BLOCK WIDTH]" > results.txt
	echo >> results.txt

	echo "[1][100][0.01]" >> results.txt
	./$(FILE:.c=.exe) 0.01 100 >> results.txt
	echo >> results.txt 

	echo "[2][1000][0.0001]" >> results.txt
	./$(FILE:.c=.exe) 0.0001 1000 >> results.txt
	echo >> results.txt 

	echo "[3][2000][0.00000000001]" >> results.txt
	./$(FILE:.c=.exe) 0.000000001 2000 >> results.txt
	echo >> results.txt 

	echo "[4][5000][0.000001]" >> results.txt
	./$(FILE:.c=.exe) 0.000001 5000 >> results.txt
	echo >> results.txt 

	echo "[5][5000][0.000000001]" >> results.txt
	./$(FILE:.c=.exe) 0.000000001 5000 >> results.txt
	echo >> results.txt 

	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null

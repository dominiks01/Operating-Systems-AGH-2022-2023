CXX 		:= gcc 
CXX_FLAGS 	:= -Wall -ggdb
FILE 		:= program.c

CALCULATE_FILE := calculate.c

all: test

compile: $(FILE) 
	$(CXX) $(CXX_FLAGS) $(CALCULATE_FILE) -o $(CALCULATE_FILE:.c=)
	$(CXX) $(CXX_FLAGS) $< -o  $(FILE:.c=.exe)
	
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

	echo "[3][2000][0.0001]" >> results.txt
	./$(FILE:.c=.exe) 0.0001 2000 >> results.txt
	echo >> results.txt 

	echo "[4][5000][0.000001]" >> results.txt
	./$(FILE:.c=.exe) 0.0001 5000 >> results.txt
	echo >> results.txt 

	echo "[5][5000][0.0001]" >> results.txt
	./$(FILE:.c=.exe) 0.0001 5000 >> results.txt
	echo >> results.txt 

	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null

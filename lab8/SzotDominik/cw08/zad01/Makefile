CC 		:= gcc
CFLAGS 	:= -Wall

FILE 	:= main.c 
SRC 	:= src
BIN 	:= bin
LIB 	:= ./lib
INCLUDE := grid.c

all: run

run: clean
	make compile 
	./$(BIN)/$(FILE:.c=)
		
compile: ./$(SRC)/$(FILE)
	$(CC) $(CFLAGS) $< -o $(BIN)/$(FILE:.c=) $(LIB)/$(INCLUDE) -lcurses
	
clean:
	rm -f ./bin/main 2> /dev/null

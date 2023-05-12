CXX 		:= gcc 
CXX_FLAGS 	:= -Wall -ggdb

BIN 	:= bin
SRC 	:= src/lab5/zad3
INCLUDE := include 

LIBRARIES 	:= 
EXECUTABLE 	:= main 

all: $(BIN)/$(EXECUTABLE)

run: clean all 
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/program.c 
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	rm -f $(BIN)/*

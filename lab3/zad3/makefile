CC = gcc -Wall 
FILE = zadanie3.c
CFLAGS = -Wall

all: test

compile: $(FILE) 
	$(CC) $(CFLAGS) $< -o  $(FILE:.c=.exe)
	
test: clean compile
	# make compile
	./$(FILE:.c=.exe) /usr "#include" 
	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null


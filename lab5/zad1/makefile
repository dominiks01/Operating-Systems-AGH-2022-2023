CC = gcc -Wall 
FILE = program.c
CFLAGS = -Wall

all: test

compile: $(FILE) 
	$(CC) $(CFLAGS) $< -o  $(FILE:.c=.exe)
	
test: clean compile
	# make compile

	echo "BY DATE: "
	./$(FILE:.c=.exe) date 

	echo "BY USER: "
	./$(FILE:.c=.exe) sender 

	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null

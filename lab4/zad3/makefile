CC = gcc -Wall 
CATCHER_FILE = catcher.c
SENDER_FILE = sender.c
CFLAGS = -Wall

all: test

compile: $(SENDER_FILE) $(CATCHER_FILE)
	$(CC) $(CFLAGS) $(CATCHER_FILE) -o  $(CATCHER_FILE:.c=.exe)
	$(CC) $(CFLAGS) $(SENDER_FILE) -o  $(SENDER_FILE:.c=.exe)
	
test: clean compile
	# make compile

	
clean:
	rm -f *.exe *.txt  *.out *.in 2> /dev/null

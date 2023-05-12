SHELL := /bin/bash

CC 		:= gcc
FLAGS 	:= -Wall
FILE 	:= main.c 
MAIN 	:= main

all: test 

test: 
	make compile
	./main 5

compile: $(FILE)
	$(CC) $(FLAGS) $(FILE) -o $(MAIN)
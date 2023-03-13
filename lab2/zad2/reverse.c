#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void reverse(char* inputFileName, char* outputFileName){
    
}


int main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(stderr, "Invalid number of arguments!\n");
        exit(EXIT_FAILURE);
    };

    replaceCharacters(argv[1],argv[2]);
}
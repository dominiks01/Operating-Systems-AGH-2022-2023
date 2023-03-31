#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
    if(argc != 2){
        fprintf(stderr, "Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", argv[0]);
    
    pid_t child_pid;

    if((child_pid = fork()) == 0){
        execl("/bin/ls", "ls", "-l", argv[1], NULL);
        exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}
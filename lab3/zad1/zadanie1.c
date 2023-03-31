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

    pid_t child_pid;

    for(int i = 0; i < atoi(argv[1]); ++i){
        if((child_pid = fork()) == 0){
            printf("PID: %i : PPID: %i\n", (int)getpid(), (int)getppid());
            exit(0);
        }

    }

    child_pid = wait(NULL);
    printf("argv:%s\n", argv[1]);

    return 0;
}
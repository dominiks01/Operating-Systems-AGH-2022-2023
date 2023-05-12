#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define SIGNAL SIGUSR1
#define STATES_START 2

void print_menu(){
    printf("USAGE: <PROGRAM> <PID> <COMMAND>\n");
    printf("COMMANDS:\n");
    printf("[1] - Print numbers in range [1, 100] one per line\n");
    printf("[2] - Print actual time [DD/MM/YYYY HH:MM:SS]\n");
    printf("[3] - Print number of state change requests\n");
    printf("[4] - Print actual time until state change\n");
    printf("[5] - Close Catcher program\n");
    
}

void handle(int signal, siginfo_t* info, void* context){
    printf("SIGNAL HAS BEN DELIVERED \n");
}

int main(int argc, char** argv) {

    if(argc <= 2){
        fprintf(stderr, "Wrong number of arguments!\n");
        print_menu();
        return  1;
    }

    int catcher_pid = atoi(argv[1]);

    struct sigaction signals;
    sigemptyset(&signals.sa_mask);
    signals.sa_sigaction = handle;

    signals.sa_flags = SA_SIGINFO;
    sigaction(SIGNAL, &signals, NULL);

    for(int i = 2; i< argc; i++){
        
        if(atoi(argv[i]) < 1 || atoi(argv[i]) > 5){
            fprintf(stderr, "Unknown command!\n");
            print_menu();
            return 1;
        }

        sigval_t state = {atoi(argv[i])};

        sigqueue(catcher_pid, SIGUSR1, state);
        sigsuspend(&signals.sa_mask);
    }



    return 0;
}
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define SIGNAL SIGUSR1

#define COUNT_TO_100 1
#define PRINT_ACTUAL_TIME 2
#define COUNT_STATE_CHANGE_REQUESTS 3
#define COUNT_ACTUAL_TIME_UNTIL_STATE_CHANGE 4
#define CLOSE_CATCHER 5

int current_request = 0;
int counter = 0;

struct command{
    char *str;
    int n; 
};

void counter_function(){
    for(int i = 1; i <= 100; ++i)
        printf("[COUNTER COMMAND]: %i \n", i);
    printf("\n");
}

void get_actual_time(){
    time_t rawtime = time(NULL);

    if(rawtime == -1){
        fprintf(stderr, "The time() function failed!\n");
    }

    struct tm* ptm = localtime(&rawtime);

    if(ptm == NULL){
        fprintf(stderr, "The time() function failed!\n");
    }

    printf("[ACTUAL TIME]: %02d/%02d/%04d %02d:%02d:%02d\n", ptm->tm_mday, ptm->tm_mon+1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

void print_counter(){
    printf("[NUMBER OR STATE CHANGE REQUESTS] : %i\n\n", counter);
}

void handle(int signal, siginfo_t* info, void* context){

    printf("RECEIVED SIGNAL!\n");
    printf("SIGNAL STATUS: [%d]\n", info->si_status);

    current_request = info->si_status;
    sigval_t state = {atoi("1")};
    counter++;

    switch (current_request) {
        case COUNT_TO_100:
            counter_function();
        break;

        case PRINT_ACTUAL_TIME:
            get_actual_time();
            printf("\n");
        break;

        case COUNT_STATE_CHANGE_REQUESTS:
            print_counter();
        break;

        case COUNT_ACTUAL_TIME_UNTIL_STATE_CHANGE:
        break;

        case CLOSE_CATCHER:
            printf("CLOSING CATCHER!\n");
        break;

        default:
            fprintf(stderr, "RECEIVED UNKNOWN COMMAND");
            exit(EXIT_FAILURE);
        break;
    }
    sigqueue(info->si_pid, SIGUSR1, state);
}

int main(){
    printf("Running catcher...\n [PID] : %i\n", getpid());

    struct sigaction signals;
    sigemptyset(&signals.sa_mask);
    signals.sa_sigaction = handle;

    signals.sa_flags = SA_SIGINFO;
    sigaction(SIGNAL, &signals, NULL);

    while(current_request!=CLOSE_CATCHER){
        if(current_request == 4)
            get_actual_time();

        sleep(1);
    }    


}
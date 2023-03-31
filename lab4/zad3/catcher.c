#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define SIGNAL SIGUSR1

void handler(int signo, siginfo_t* info, void* context) {
    
}


int main(){
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGNAL, &action, NULL);
}
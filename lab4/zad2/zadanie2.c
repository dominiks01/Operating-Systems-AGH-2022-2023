#include <bits/types/siginfo_t.h>
#include <bits/types/sigset_t.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int call_id;
int call_depth;

#define MAX_CALLS 4

typedef void (*handler_act)(int, siginfo_t*, void*);

void au(int sig_no) {
    printf("Otrzymale signal %d.\n", sig_no);
}

void info_handler(int signal, siginfo_t* info, void* context){
    printf("INFO HANDLER: SIGNO [%d]\n", info->si_signo);
    printf("INFO HANDLER: SSIGNAL PID [%d]\n", info->si_pid);
    printf("INFO HANDLER: SIGNAL UID [%d]\n", info->si_uid);
}

void set_sigaction(int signal_id, struct sigaction action, int flags, handler_act handler){
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    action.sa_flags = flags;
    sigaction(signal_id, &action, NULL);

    call_id = 0;
    call_depth = 0;
}

void depth_handler(int signo, siginfo_t* info, void* context) {
    int current_id = call_id;
    printf("SIGNAL HANDLER START: ID [%d] DEPTH: [%d]\n", current_id, call_depth);
    call_id ++;
    call_depth ++;

    if(call_id < MAX_CALLS){
        raise(SIGUSR1);
    }

    call_depth--;

    printf("SIGNAL HANDLER END: ID [%d] DEPTH: [%d]\n", current_id, call_depth);
}

void set_restart(struct sigaction act){
    set_sigaction(SIGUSR1, act, SA_RESTART, depth_handler);
}

void set_nodefer(struct sigaction act){
    set_sigaction(SIGUSR1, act, SA_NODEFER, depth_handler);
}

void set_info(struct sigaction act){
    set_sigaction(SIGUSR1, act, SA_SIGINFO, info_handler);
}

int main(int argc, char** argv){

    printf("\nSIGINFO FLAG\n");
    struct sigaction act;
    set_info(act);  
    raise(SIGUSR1);

    printf("\nNODEFER FLAG\n");
    struct sigaction act2;
    set_nodefer(act2);
    raise(SIGUSR1);

    printf("\nRESTART FLAG\n");
    struct sigaction act3;
    set_restart(act3);
    raise(SIGUSR1);

    return 0;
}
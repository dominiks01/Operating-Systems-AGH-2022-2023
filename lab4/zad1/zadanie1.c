#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define IGNORE 1 
#define HANDLER 2 
#define MASK 3
#define PENDING 4

#define FORK 10
#define EXEC 11

#define SIGNAL SIGUSR1

struct command{
    char *str;
    int n; 
};

int compare(const void *s1, const void *s2){
    const struct command *e1  = s1; 
    const struct command *e2  = s2;

    return strcmp(e1->str, e2->str); 
}

struct command dict[] = {
    {"ignore",    IGNORE}, 
    {"handler",   HANDLER},
    {"mask",  MASK},
    {"pending",    PENDING},
};

struct command functions[] = {
    {"fork",    FORK}, 
    {"exec",   EXEC},
};

void raise_signal() {
    printf("[%d] Raising signal\n", getpid());
    raise(SIGNAL);
}

void signal_handler(int signum) {
    printf("[%d] Handling signal\n", getpid());
}

void block_signal() {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGNAL);
    sigprocmask(SIG_BLOCK, &action.sa_mask, NULL);
}

void print_is_pending() {
    sigset_t sigset;
    sigpending(&sigset);
    if (sigismember(&sigset, SIGNAL))
        printf("[%d] Signal pending\n", getpid());
    else
        printf("[%d] Signal not pending\n", getpid());
}

void process_signal(struct command result, int pid){
    switch (result.n){
        case IGNORE:
            signal(SIGNAL, SIG_IGN);
            raise_signal();
            break;
        
        case HANDLER:
            signal(SIGNAL, signal_handler);
            raise_signal();
            break;
        
        case MASK:        
        case PENDING:
            signal(SIGNAL, signal_handler);

            if(pid == getpid()){
                block_signal();        
                raise_signal();
            }

            if(result.n == PENDING)
                print_is_pending();
                
            break;
  
        default:
            fprintf(stderr, "Unknown command!");
            break;    
        }
}

void process_function(int argc, char * argv[], struct command function_r, struct command command_r, int PID){
    int is_parent;

    if(command_r.n == HANDLER && function_r.n == EXEC){
        fprintf(stderr, "Handler not available in EXEC function\n");
        exit(EXIT_FAILURE);
    }
    
    switch (function_r.n){
        case FORK:
            process_signal(command_r, PID);

            if (!(is_parent = fork()))
                process_signal(command_r, PID);
            else
                wait(NULL);

            break;

        case EXEC:
            process_signal(command_r, PID);

            if(argc == 3)
                execl(argv[0], argv[0], argv[1], argv[2], "NULL", NULL);

            break;
        
        default:
            fprintf(stderr, "Unknown function!\n");
            break;    
    }
}

struct command* map_function_to_int(char * arg){
    struct command *result, key = {arg};
    result = bsearch(&key, functions, sizeof(functions)/sizeof(functions[0]),
                        sizeof functions[0], compare);

    return result;
}

struct command* map_command_to_int(char * arg){
    struct command *result, key = {arg};
    result = bsearch(&key, dict, sizeof(dict)/sizeof(dict[0]),
                        sizeof dict[0], compare);

    return result;
}

int main(int argc, char * argv[]){

    int PID = getpid();

    if (argc > 4 || argc <= 2){
        fprintf(stderr, "%s", "Wrong number of arguments! Use <program> <SIGNAL> <FUNCTION>\n");
        return 1;
    }

    qsort(dict, sizeof(dict)/sizeof(dict[0]), sizeof dict[0], compare);
    qsort(functions, sizeof(functions)/sizeof(functions[0]), sizeof functions[0], compare);

    struct command *result = map_command_to_int(argv[1]);
    struct command *function_result = map_function_to_int(argv[2]);

    if(result == NULL){
        fprintf(stderr,"Unknown command!\n");
        return 1;
    } 

    if(function_result == NULL){
        fprintf(stderr,"Unknown function! Should be fork|exec\n");
        return 1;
    } 

    process_function(argc, argv, *function_result,*result, PID);
        
    return 0;
}


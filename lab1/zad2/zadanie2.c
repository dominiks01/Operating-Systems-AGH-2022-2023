#include "memoryBlock.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <dlfcn.h>

struct command{
    char *str;
    int n; 
};

struct command dict[] = {
    {"count",    2}, 
    {"delete",   4},
    {"destroy",  5},
    {"init",    1},
    {"show",     3}, 
};

int compare(const void *s1, const void *s2){
    const struct command *e1  = s1; 
    const struct command *e2  = s2;

    return strcmp(e1->str, e2->str); 
}

struct tms cpu_start;
struct tms cpu_end;
struct timespec clock_start; 
struct timespec clock_end;

void start_clock(void){
    clock_gettime(CLOCK_REALTIME, &clock_start);
    times(&cpu_start);
};

int validateCommand(char * command){
    char *commands[] = {"init", "count", "delete", "destroy", "show"};
    for(int i = 0; i < 5; ++i)
        if(strcmp(commands[i], command) == 0)
            return 1; 
    return 0;
}

void end_clock(){
    clock_gettime(CLOCK_REALTIME, &clock_end);
    times(&cpu_end);

    clock_t cpu_time1 = cpu_end.tms_utime - cpu_start.tms_utime;
    clock_t cpu_time2 = cpu_end.tms_stime - cpu_start.tms_stime;

    printf("\nReal:   %lf ns\nUser:   %jd ticks\nSystem: %jd ticks\n\n\n",
        (double)(clock_end.tv_nsec - clock_start.tv_nsec),
        (__intmax_t)cpu_time1,
        (__intmax_t)cpu_time2);
};


int main(){
    char line [255];
    char *command;
    char delimiters[] = " "; 
    dataBlock* data = NULL; 

    while (fgets(line, 255, stdin) != NULL){
        command = strtok(line, delimiters);
        command[strcspn(command, "\n")] = 0;

        if(validateCommand(command) == 0){
            printf("Unknown command!");
            continue;
        }

        struct command *result, key = {command};
        result = bsearch(&key, dict, sizeof(dict)/sizeof(dict[0]), sizeof dict[0], compare);

        switch (result->n){
        case 1:
            command = strtok(NULL, delimiters);
            int size = atoi(command);
            start_clock();
            data = initializeNewBlock(size);
            end_clock();
            break;
        
        case 2:
            command = strtok(NULL, delimiters);
            command[strcspn(command, "\n")] = 0;
            start_clock();
            countLinesAndWords(data, command);
            end_clock();
            break;
        
        case 3:
            command = strtok(NULL, delimiters);
            int index = atoi(command);
            start_clock();
            char* answer = getBlock(data, index);
            printf("%s", answer);
            end_clock();
            break;
        
        case 4:
            command = strtok(NULL, delimiters);
            index = atoi(command);
            start_clock();
            deleteBlock(data, index);
            end_clock();
            break;

        case 5:
            start_clock();
            deleteDataBlock(data);
            end_clock();
            break;
        
        default:
            printf("Unknown command!");
            break;
        }
    }
    return 0;
}
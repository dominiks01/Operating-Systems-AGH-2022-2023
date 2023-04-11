#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFSIZE 1024
#define FIFO_PATH "/tmp/new_fifo"


double* split_array(double *array, int n, double block_size){
    double start = 0;
    array[0] = start;

    for(int i = 0; i < n; i++)
        array[i+1] = array[i] + (int)(((i+1)*(1./n) - array[i])/block_size) * block_size;

    return array;
}

void handle(int signal, siginfo_t* info, void* context){
    printf("RECEIVED SIGNAL!\n");    
}

double split_calculation(int number_of_children, double block_width){
    double *range = calloc(number_of_children+1, sizeof(double));
    range = split_array(range, number_of_children, block_width);

    char read_buffer[BUFFSIZE] = "";
    char starting_range[BUFFSIZE] = "";
    char ending_range[BUFFSIZE] = "";
    char block_argument[BUFFSIZE] = "";

    double result = 0;
    mkfifo(FIFO_PATH, 0666);

    for(int i = 0; i < number_of_children; i++){
        id_t pid = fork();

        if(pid == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            snprintf(starting_range, BUFFSIZE, "%lf", range[i]);
            snprintf(ending_range, BUFFSIZE, "%lf", range[i+1]);
            snprintf(block_argument, BUFFSIZE, "%lf", block_width);
            execl("./calculate", "calculate",starting_range, ending_range, block_argument, getppid(), NULL);
        }
    }

    int fifo = open(FIFO_PATH, O_NONBLOCK, O_RDONLY);
    int already_read = 0;

    while (1) {
        size_t size = read(fifo, read_buffer, BUFFSIZE);
        read_buffer[size] = 0;

        char delim[] = "\n";
        char* token;

        token = strtok(read_buffer, delim);
        for (;token; token = strtok(NULL, delim)) {
            result += strtod(token, NULL);
            printf("READ: [%lf]\n", strtod(token, NULL));
            already_read++;
        }

        sleep(1);
    }

    close(fifo);
    remove(FIFO_PATH);
    return  result;
}

int main(int argc, char ** argv){
    printf("%d\n", getpid());

    if(argc != 3){
        fprintf(stderr,"Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    clock_t tic = clock();
    split_calculation(atoi(argv[2]), strtod(argv[1], NULL));
    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    
    return  0;
}
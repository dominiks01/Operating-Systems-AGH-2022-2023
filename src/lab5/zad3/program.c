#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define BUFFSIZE 50000
#define FIFO_PATH "/tmp/new_fifo"

double* split_array(double *array, int n, double block_size){
    double start = 0;
    array[0] = start;

    for(int i = 0; i < n; i++)
        array[i+1] = array[i] + (int)(((i+1)*(1./n) - array[i])/block_size) * block_size;

    return array;
}

double split_calculation(int number_of_children, double block_width){

    mkfifo(FIFO_PATH, 0666);
    int fifo = open(FIFO_PATH, O_NONBLOCK, O_RDONLY);

    double *range = calloc(number_of_children+1, sizeof(double));
    range = split_array(range, number_of_children, block_width);

    char read_buffer[BUFFSIZE] = "";
    char starting_range[BUFFSIZE] = "";
    char ending_range[BUFFSIZE] = "";
    
    char block_argument[BUFFSIZE] = "";

    double result = 0;
    id_t pid;

    for(int i = 0; i < number_of_children; i++){
        pid = fork();

        if(pid == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            snprintf(starting_range, BUFFSIZE, "%lf", range[i]);
            snprintf(ending_range, BUFFSIZE, "%lf", range[i+1]);
            snprintf(block_argument, BUFFSIZE, "%lf", block_width);
            int cx = execl("./calculate", "calculate", starting_range, ending_range, block_argument, NULL);
        }
    }

    while(wait(NULL) > 0);
    int file = read(fifo, read_buffer, BUFFSIZE);

    if(file == -1){
        fprintf(stderr, "Cannot read file!\n");
        exit(EXIT_FAILURE);
    }

    char delimiter[] = "\n";
    char *token = strtok(read_buffer, delimiter);

    while (token != NULL) {
        result += strtod(token, NULL);
        token = strtok(NULL,delimiter);
    }

    close(fifo);
    remove(FIFO_PATH);

    printf("RESULT: [%lf]\n", result);
    return  result;
}

int main(int argc, char ** argv){

    if(argc != 3){
        fprintf(stderr,"Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    split_calculation(atoi(argv[2]), strtod(argv[1], NULL));
    clock_gettime(CLOCK_REALTIME, &end);
    double real_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 0.000000001;
    printf("EXECUTION TIME: %f s\n", real_time);
    
    return  0;
}
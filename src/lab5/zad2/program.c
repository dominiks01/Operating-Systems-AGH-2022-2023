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

double function_value(double x){
    return 4./(x*x + 1);
}

double* split_array(double *array, int n, double block_size){
    double start = 0;
    array[0] = start;

    for(int i = 0; i < n; i++)
        array[i+1] = array[i] + (int)(((i+1)*(1./n) - array[i])/block_size) * block_size;

    return array;
}

double calculate(double a, double b, double block_width){
    double result = 0;

    for(double i = a; i+block_width <= b; i+= block_width)
        result += function_value(i) * block_width;
        
    return result;
}

double split_calculation(int number_of_children, double block_width){
    double *range = calloc(number_of_children+1, sizeof(double));
    range = split_array(range, number_of_children, block_width);

    double result = 0;
    int fd[number_of_children][2];
    id_t pid;
    double out;

    for(int i = 0; i < number_of_children; i++){
        pipe(fd[i]);
        pid = fork();

        if(pid == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            close(fd[i][0]);
            out = calculate(range[i], range[i+1], block_width);
            write(fd[i][1],&out, sizeof(double));
            exit(0);
        } else {
            wait(NULL);
            close(fd[i][1]);
            read(fd[i][0], &out, sizeof(double));
            result += out;
        }
    }

    printf("RESULT %lf \n", result);
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
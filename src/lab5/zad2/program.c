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

#define BUFFSIZE 1024

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

    char read_buffer[BUFFSIZE] = "";
    char write_buffer[BUFFSIZE] = "";

    double result = 0;

    for(int i = 0; i < number_of_children; i++){
        int fd[2];
        pipe(fd);
        id_t pid = fork();

        if(pid == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            close(fd[0]);
            double out = calculate(range[i], range[i+1], block_width);
            size_t size = snprintf(write_buffer,BUFFSIZE, "%f", out);
            write(fd[1],write_buffer, size);
            exit(0);
        } else {
            close(fd[1]);
            read(fd[0], read_buffer, BUFFSIZE);
            result += strtod(read_buffer, NULL);
        }

        while (wait(NULL) > 0);
    }

    printf("RESULT %lf \n", result);
    return  result;
}


int main(int argc, char ** argv){

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
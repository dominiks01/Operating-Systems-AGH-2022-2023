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
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define BUFFSIZE 1024
#define FIFO_PATH "/tmp/new_fifo"

double function_value(double x){
    return 4./(x*x + 1);
}

double test(double a, double b, double block_width){
    double result = 0;

    for(double i = a; i < b; i += block_width){
        result += function_value(i) * block_width;
    }

    // printf("%lf %lf %lf [%lf]\n",a,b,block_width, result);
    return result;
}

int main(int argc, char** argv){
    if(argc != 4){
        fprintf(stderr, "Wrong number of arguments: %d !\n", argc);
        return 1;
    }

    double a = strtod(argv[1], NULL);
    double b = strtod(argv[2], NULL);
    double block_width = strtod(argv[3], NULL);

    char write_buffer[BUFFSIZE] = "";
    
    double result = test(a,b,block_width);
    size_t size = snprintf(write_buffer, BUFFSIZE, "%lf\n", result);

    int fifo = open(FIFO_PATH, O_WRONLY);
    write(fifo, write_buffer, size);
    return 0;
}
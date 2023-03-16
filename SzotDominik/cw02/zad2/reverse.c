#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct tms cpu_start;
struct tms cpu_end;
struct timespec clock_start; 
struct timespec clock_end;

void start_clock(void){
    clock_gettime(CLOCK_REALTIME, &clock_start);
    times(&cpu_start);
};

void end_clock(void){
    clock_gettime(CLOCK_REALTIME, &clock_end);
    times(&cpu_end);

    clock_t cpu_time1 = cpu_end.tms_utime - cpu_start.tms_utime;
    clock_t cpu_time2 = cpu_end.tms_stime - cpu_start.tms_stime;

    FILE* pomiar = fopen("pomiar_zad_2.txt","a+b");

    char result [100];
    snprintf(result,100, "\nReading char by char\nReal:   %lf ns\nUser:   %jd ticks\nSystem: %jd ticks\n\n",
        (double)(clock_end.tv_nsec - clock_start.tv_nsec),
        (__intmax_t)cpu_time1,
        (__intmax_t)cpu_time2);

    fwrite(result, sizeof(char), strlen(result), pomiar);
    fclose(pomiar);
};

void reverse(char* inputFileName, char* outputFileName){
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "a+b");

    char c; 
    fseek(inputFile, -1, SEEK_END);

    while ((ftell(inputFile)) != 0){
        fread(&c, sizeof(char), 1, inputFile);
        fwrite(&c, sizeof c, 1, outputFile);
        fseek(inputFile, -2, SEEK_CUR);
    }

    fread(&c, sizeof c, 1, inputFile);
    fwrite(&c, sizeof c, 1, outputFile);

    fclose(inputFile);
    fclose(outputFile);
}


int main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(stderr, "Invalid number of arguments!\n");
        exit(EXIT_FAILURE);
    };

    start_clock();
    reverse(argv[1],argv[2]);
    end_clock();

    return 0;
}
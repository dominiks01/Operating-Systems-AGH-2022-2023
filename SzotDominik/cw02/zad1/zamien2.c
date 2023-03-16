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

    FILE* pomiar = fopen("pomiar_zad_1.txt","a+b");

    char result [100];
    snprintf(result,100, "\nUsing library's fread() & fwrite()\nReal:   %lf ns\nUser:   %jd ticks\nSystem: %jd ticks\n\n",
        (double)(clock_end.tv_nsec - clock_start.tv_nsec),
        (__intmax_t)cpu_time1,
        (__intmax_t)cpu_time2);

    fwrite(result, sizeof(char), strlen(result), pomiar);
    fclose(pomiar);
};

void replaceCharacters(char* search, char* replace, char* inputFileName, char* outputFileName){
    start_clock();

    FILE * openFile = fopen(inputFileName, "r");

    if(openFile == NULL){
        fprintf(stderr, "Cannot open %s - no such file!\n", inputFileName);
        exit(EXIT_FAILURE);
    }
    FILE * outputFile = fopen(outputFileName, "a+b");

    if(outputFile == NULL){
        fprintf(stderr, "Cannot create file %s!\n", outputFileName);
        exit(EXIT_FAILURE);
    }

    char buffer; 
    int writeHandle;

    while (fread(&buffer, sizeof buffer, 1, openFile)){
        if(buffer == *search)
            buffer = *replace;

        writeHandle = fwrite(&buffer, sizeof buffer, 1, outputFile);

        if(writeHandle < 0){
            fprintf(stderr, "sth went wrong with writint to %s file!\n", outputFileName);
            exit(EXIT_FAILURE);
        }
    }

    end_clock();
    fclose(openFile);
    fclose(outputFile);
}

int main(int argc, char *argv[]){

    if(argc != 5){
        fprintf(stderr, "Invalid number of arguments!\n");
        exit(EXIT_FAILURE);
    };

    replaceCharacters(argv[1],argv[2],argv[3],argv[4]);

    return 0;
}
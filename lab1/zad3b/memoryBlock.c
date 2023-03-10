#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "memoryBlock.h"

dataBlock* initializeNewBlock(int numberOfElements){
    dataBlock * newMemoryBlock = calloc(1, sizeof(dataBlock));
    newMemoryBlock->memory = calloc( numberOfElements, sizeof(char*));

    newMemoryBlock->size = 0;
    newMemoryBlock->capacity = numberOfElements;

    return newMemoryBlock;
}

void countLinesAndWords(dataBlock* data, char* filename){

    // Making command for word counter using snprintf 
    char command [256];
    char resultFileName [245];

    sprintf(resultFileName, "/tmp/%s-wc-result.txt", filename);
    sprintf(command, "wc -clw %s > %s", filename, resultFileName);

    system(command);

    int indexToWrite = -1;

    for(int i = 0; i < data->capacity; ++i)
        if(data->memory[i] == NULL){
            indexToWrite = i; 
            break;
        }
    
    if(indexToWrite == -1){
        fprintf(stderr, "Not enought space!\n");
        exit(EXIT_FAILURE);
    }
    
    FILE *fp = fopen(resultFileName, "r");

    if (fp == NULL){
        fprintf(stderr, "Cannot open file!\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // Reopening content file to write into out memory block  
    data->memory[indexToWrite] = (char*)calloc(size, sizeof(char*));

    fread(data->memory[indexToWrite], sizeof(char), size, fp);    
    data->size += 1;

    // Removing unnecessary result file
    char rm_command [255];
    snprintf(rm_command, 255, "rm -r %s", resultFileName);
    system(rm_command);

    fclose(fp);
}

char* getBlock(dataBlock* data, int index){

    if(index > data->capacity || index < 0){
        fprintf(stderr, "Index out of range!\n");
        exit(EXIT_FAILURE);
    }

    if(data->memory[index] == NULL){
        fprintf(stderr, "No such element!\n");
        exit(EXIT_FAILURE);
    }

    return data->memory[index];
}

void deleteBlock(dataBlock* data, int index){
     if(index > data->capacity || index < 0){
        fprintf(stderr, "Index out of range!\n");
        exit(EXIT_FAILURE);
    }
    if(data->memory[index] != NULL)
        data->memory[index] = NULL;
    free(data->memory[index]);
}

void deleteDataBlock(dataBlock* data){
    for(int i = 0; i < data->capacity; i++)
        deleteBlock(data, i);
    free(data);
}

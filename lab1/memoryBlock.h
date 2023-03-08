#include <stdio.h>

typedef struct{
    char** memory; 
    size_t size;
    size_t capacity; 
} dataBlock; 

dataBlock* initializeNewBlock(size_t);

void countLinesAndWords(dataBlock*, char*);

char* getBlock(dataBlock*, int);

void deleteBlock(dataBlock*, int);

void deleteDataBlock(dataBlock*);

#ifndef MEMORY_BLOCK_H_
#define MEMORY_BLOCK_H_

typedef struct{
    char** memory; 
    int size;
    int capacity; 
} dataBlock;

dataBlock* initializeNewBlock(int);
void countLinesAndWords(dataBlock*, char*);
char* getBlock(dataBlock*, int);
void deleteBlock(dataBlock*, int);
void deleteDataBlock(dataBlock*);

#endif
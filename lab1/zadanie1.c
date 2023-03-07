#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// typedef struct{
//     int vaild; 
//     char *content; 
// } memoryBlock;

typedef struct{
    char ** memory; 
    size_t size;
    size_t capacity; 
} dataBlock; 


dataBlock* initializeNewBlock(size_t numberOfElements){
    dataBlock * newMemoryBlock = calloc(sizeof(dataBlock), 1);
    newMemoryBlock->memory = calloc(sizeof(dataBlock), numberOfElements);

    newMemoryBlock->size = 0;
    newMemoryBlock->capacity = numberOfElements;

    return newMemoryBlock;
}

void countLinesAndWords(dataBlock* data, char* filename){

    // Making command for word counter using snprintf 
    char command [255];
    int cx = snprintf(command, 255, "wc --chars --lines --words %s > /tmp/result.txt", filename);

    system(command);
    int indexToWrite = -1;

    for(int i = 0; i < data->capacity; ++i)
        if(data->memory[i] == 0){
            indexToWrite = i; 
            break;
        }
    
    if(indexToWrite == -1)
        return;
    
    FILE *fp; 
    fp = fopen("/tmp/result.txt", "r");

    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // Reopening content file to write into out memory block  
    data->memory[indexToWrite] = (char*)calloc(sizeof(char*), size);

    fread(data->memory[indexToWrite], sizeof(char), size, fp);    
    data->size += 1;

    // Removing unnecessary result file
    char* rm_command = {"rm -r /tmp/result.txt"};
    system(rm_command);

    fclose(fp);
}

char* getBlock(dataBlock* data, int index){
    return data->memory[index];
}

void deleteBlock(dataBlock* data, int index){
    if(data->memory[index] == NULL)
        return;
    free(data->memory[index]);
    data->memory[index] = NULL;
}

void deleteDataBlock(dataBlock* data){
    for(int i = 0; i < data->capacity; i++)
        deleteBlock(data, i);
    free(data);
}



int main(){
    dataBlock * newData = initializeNewBlock(10);
    countLinesAndWords(newData, "test.txt");
    char * block = getBlock(newData, 0);
    puts(block);
    deleteBlock(newData, 0);
    deleteDataBlock(newData);
    return 0;
}
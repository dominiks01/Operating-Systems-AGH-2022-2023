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
    int cx; 
    cx = snprintf(command, 255, "wc --chars --lines --words %s > /tmp/result.txt", filename);

    system(command);
    
    // Getting number of byts for file content from result of wc command
    FILE *fp; 
    fp = fopen("/tmp/result.txt", "r");

    char line[255];
    fgets(line,255, fp);

    char * result; 
    result = strtok(line, " ");
    result = strtok(NULL, " ");
    result = strtok(NULL, " ");

    size_t size;
    sscanf(result, "%zu", &size);
    
    // Reopening content file to write into out memory block  
    fclose(fp);
    fp = fopen(filename, "r");
    data->memory[data->size] = (char*)calloc(sizeof(char*), size);

    // char* buffer = (char*)calloc(size, sizeof(char));
    fread(data->memory[data->size], sizeof(char), size, fp);

    //memoryBlock * block = calloc(sizeof(memoryBlock), 1);
    //block->content =  calloc(sizeof(char*), size+1);
    // block = calloc(sizeof(memoryBlock), 1);

    // data->memory[data->size] = buffer;
    //data->memory[data->size] = (char*)calloc(sizeof(char*), size);
    //strcpy(data->memory[data->size], buffer);

    // strcpy(block->content, buffer);
    // data->memory[data->size] = block;
    
    data->size += 1;

    // Removing unnecessary result file
    char* rm_command = {"rm -r /tmp/result.txt"};
    system(rm_command);

    fclose(fp);
}

char* getBlock(dataBlock* data, int index){
    return data->memory[index];
}

void* deleteBlock(dataBlock* data, int index){
    data->size -= 1;
    free(data->memory[index]);
}


int main(){
    dataBlock * newData = initializeNewBlock(10);
    countLinesAndWords(newData, "test.txt");
    char * block = getBlock(newData, 0);
    puts(block);


    free(newData);
    free(block); 
    return 0;
}
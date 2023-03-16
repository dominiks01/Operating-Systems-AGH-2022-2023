#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(){
    DIR* directory = opendir(".");
    struct dirent* dirStructure;
    long long totalSize = 0;

    while( (dirStructure=readdir(directory)) != NULL){

        struct stat* fileStructure = malloc(sizeof(struct  stat));
        int result = stat(dirStructure->d_name, fileStructure);

        if(result == -1)
            printf("sth went wrong\n");

        if(!S_ISDIR(fileStructure->st_mode)){
            printf("file name: \"%s\"", dirStructure->d_name);
            printf(" size %li \n", fileStructure->st_size);

            totalSize += fileStructure->st_size;
        }

        free(fileStructure);
    }
    printf("Total size: %lli\n\n", totalSize);
    closedir(directory);
}
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr, "Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    DIR* directory = opendir(argv[1]);
    struct dirent* dirStructure;

    pid_t child_pid;

    while( (dirStructure=readdir(directory)) != NULL){
        struct stat* fileStructure = malloc(sizeof(struct  stat));

        if(strcmp(dirStructure->d_name, ".") == 0 || strcmp(dirStructure->d_name, "..") == 0 ) {
            free(fileStructure);
            continue;
        }

        char* pathToNextObject = malloc(strlen(argv[1]) + strlen(dirStructure->d_name) + 2);
        snprintf(pathToNextObject, strlen(argv[1]) + strlen(dirStructure->d_name) + 2, "%s/%s", 
            argv[1], dirStructure->d_name);
    
        int result = stat(pathToNextObject, fileStructure);

        if(S_ISLNK(fileStructure->st_mode)){
            free(fileStructure);
            continue;
        }

        // if(access(pathToNextObject, R_OK) != -1){
        //     free(fileStructure);
        //     continue;
        // }

        if(result != 0){
            fprintf(stderr, "%d\n", result);
            printf("ERROR: %s %d\n", pathToNextObject, fileStructure->st_mode);
            free(fileStructure);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(fileStructure->st_mode)){
            if((child_pid = fork()) == 0){
                execl(argv[0], "debbuging", pathToNextObject, argv[2], NULL);
                exit(0);
            } else {
                wait(NULL);
            }
        } else {
            FILE* openfile = fopen(pathToNextObject, "r");
            char buffer[255] = {0};
            int cs = fread(&buffer, sizeof(char), strlen(argv[2]) , openfile);

            if(cs != strlen(argv[2])){
                fclose(openfile);
                break;
            }

            if(strcmp(buffer, argv[2]) == 0){
                printf("PATH: %s PID: %i\n", pathToNextObject, (int)getpid());
            }

            fclose(openfile);
        }

        free(fileStructure);
        free(pathToNextObject);    
    }   

    while(wait(NULL) > 0); 
    return 0;
}
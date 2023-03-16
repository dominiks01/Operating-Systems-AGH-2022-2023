#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ftw.h>

long long int size = 0;

static int display_info(const char* fpath,const struct stat*sb,int tflag){
	if(!S_ISDIR(sb->st_mode)){
        printf("file name: \"%s\"", fpath);
        printf(" size %li \n", sb->st_size);
        size += sb->st_size;
    }
	return 0;
}


int main(int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr, "Invalid number of arguments!\n");
        exit(EXIT_FAILURE);
    };

    if(ftw(argv[1],display_info,30)==-1){
        perror("ftw()");
		exit(-1);
    }

    printf("%lli\n\n", size);
    return 0;
}
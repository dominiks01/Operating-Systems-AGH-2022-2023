#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <err.h>
#include <errno.h>

#include "utils.h"

static int get_shared_memory(const char *filename, int size) {
    key_t key;
    if ((key = ftok(filename, 0)) == -1) {
        fprintf(stderr, "[ERROR] ftok ended with error: %s\n", strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    return shmget(key, size, 0644 | IPC_CREAT);
}

char *attach_shared_memory(const char* filename, int size) {
    int shared_memory_id = get_shared_memory(filename, size);
    char *shared_memory;
    if (shared_memory_id == -1) {
        fprintf(stderr, "[ERROR] No such file: %s %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((shared_memory = shmat(shared_memory_id, NULL, 0)) == (char *)(-1)){
        fprintf(stderr, "[ERROR] shmat error!: %d %s\n", shared_memory_id, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return shared_memory;
}

bool detach_shared_memory(char *shared_memory) {
    return (shmdt(shared_memory) != -1);
}

bool destroy_shared_memory(const char *filename) {
    int shared_memory_id = get_shared_memory(filename, 0);
    if(shared_memory_id == -1) {
        exit(EXIT_FAILURE);
    }
    return (shmctl(shared_memory_id, IPC_RMID, NULL) != -1);
}

Semaphore create_semaphore(const char *filename, int initial) {
    key_t key;
    if ((key = ftok(getenv("HOME"), filename[0])) == -1) {
        perror("Creating a semaphore failed on ftok");
        exit(EXIT_FAILURE);
    }

    Semaphore semid;
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("Creating a semaphore failed on semid");
        exit(EXIT_FAILURE);
    }
    
    if(semctl(semid, 0, SETVAL, initial) == -1) {
        perror("Creating a semaphore failed on semctl");
        exit(EXIT_FAILURE);
    }
    return semid;
}

Semaphore open_semaphore(const char *filename) {
    key_t key; 
    if((key = ftok(getenv("HOME"), filename[0])) == -1) {
        exit(EXIT_FAILURE);
    }
    return semget(key, 1, 0);
}

void close_semaphore(Semaphore sem) {}

void unlink_semaphore(const char* filename) {
    Semaphore semid;
    if((semid = open_semaphore(filename)) == -1) {
        fprintf(stderr, "Failed to unlink semaphore.\n");
        return;
    }
    semctl(semid, 0, IPC_RMID);
}

void aquire(Semaphore sem) {
    struct sembuf operation = { 0, -1, 0 };
    if(semop(sem, &operation, 1) == -1) {
        perror("aquire"); 
    }
}

void release(Semaphore sem) {
    struct sembuf operation = { 0, 1, 0 };
    if(semop(sem, &operation, 1) == -1){
        perror("aquire");
    }
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "common.h"
#include "utils.h"
#include "queue.h"

#define HAIRCUT_TIME 2000
#define TIMEOUT 1000000

static Semaphore sem_queue;
static Semaphore sem_chairs;
static Semaphore sem_barbers;
static Semaphore buffer_mutex;

char *queue;

void open_semaphores();

int main(void){
    if ((queue = attach_shared_memory(PROJECT_IDENTIFIER, BUFFER_SIZE)) == NULL){
        fprintf(stderr, "[ERROR] Can't open queue.\n");
        exit(EXIT_FAILURE);
    }
    open_semaphores();

    while (true){
        aquire(sem_barbers);
        char haircut = queue_pop(queue);

        printf("\t Barber nr. %d processing: %d\n", getpid(), haircut);
        usleep(HAIRCUT_TIME);
        printf("\t Barber nr. %d done\n", getpid());

        release(sem_chairs);
        release(sem_queue);

        if (queue_empty(queue)){
            usleep(TIMEOUT);
        }
    }
    fflush(stdout);

    detach_shared_memory(queue);
    return EXIT_SUCCESS;
}

void open_semaphores(){
    sem_queue = open_semaphore(SEM_QUEUE_FNAME);
    sem_chairs = open_semaphore(SEM_CHAIRS_FNAME);
    sem_barbers = open_semaphore(SEM_BARBERS_FNAME);
    buffer_mutex = open_semaphore(SEM_BUFFER_MUTEX_FNAME);
}
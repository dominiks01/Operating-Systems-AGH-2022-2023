#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdbool.h>

#define REINDEER_TOTAL 9
#define ELVES_TOTAL 10
#define ELVES_REQUIRED 3

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    unsigned int value;
} Monitor;

static Monitor elves = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .condition = PTHREAD_COND_INITIALIZER,
    .value = 0
};
    
static Monitor rindeers = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .condition = PTHREAD_COND_INITIALIZER,
    .value = 0
};

static Monitor santa = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .condition = PTHREAD_COND_INITIALIZER,
    .value = 0
};

void* santa_routine(void* arg){
    while (true) {
        pthread_cond_wait(&santa.condition, &santa.mutex);
        printf("Mikołaj: budzę się!\n");      
        
        pthread_mutex_lock(&elves.mutex);

        if (elves.value == 3 ) {
                printf("Mikołaj: rozwiązuję problemy elfów\n");
                sleep(rand() % 2 + 1);
                pthread_cond_signal(&elves.condition);
                elves.value = 0;
        }

        pthread_mutex_unlock(&elves.mutex);
        pthread_mutex_lock(&rindeers.mutex);

        if (rindeers.value == REINDEER_TOTAL ) {
                printf("Mikołaj: dostarczam zabawki\n");
                sleep(rand() % 3 + 2);
                santa.value++;
                pthread_cond_signal(&rindeers.condition);
                rindeers.value = 0;

                if(santa.value == 3)
                    pthread_exit(NULL);
        } 
        pthread_mutex_unlock(&rindeers.mutex);
  
    }
    printf("Mikołaj: wracam do snu!\n");
    pthread_exit(NULL);
}


void *reindeer_routine(void *arg) {
    int id =  syscall(__NR_gettid);

    while (true) {
        sleep(5 + (rand() % 6));
        pthread_mutex_lock(&rindeers.mutex);

        if(rindeers.value < REINDEER_TOTAL)
            rindeers.value ++;

        printf("Renifer: czeka [%d] reniferów na Mikołaja [%d]\n", rindeers.value, id);

        if (rindeers.value == REINDEER_TOTAL) {
            printf("Renifer: [%d] wybudzam Mikołaja.\n", id);
            pthread_cond_signal(&santa.condition);
            pthread_cond_wait(&rindeers.condition, &rindeers.mutex);
        }

        pthread_mutex_unlock(&rindeers.mutex);
    }
    return NULL;
}

void *elf_routine(void *arg) {
    int id =  syscall(__NR_gettid);

    while(true) {
        sleep(2 + (rand() % 4));
        pthread_mutex_lock(&elves.mutex);

        if (elves.value < ELVES_REQUIRED ){
            elves.value++;
            printf("Elf: [%d] elfów czeka na mikołaja. [%d]\n", elves.value ,id);

            if (elves.value == ELVES_REQUIRED) {
                printf("Elf: wybudza mikołaja [%d]\n", id);
                pthread_cond_signal(&santa.condition);
                pthread_cond_wait(&elves.condition, &elves.mutex);
            }

        } else {
            printf("Elf: [%d] samodzielnie rozwiązuję swój problem.\n", id);
        }
        pthread_mutex_unlock(&elves.mutex);
    }

    return NULL;
}

int main(int argc, char** argv) {
    pthread_t santa;
    pthread_create(&santa, NULL, santa_routine, NULL);

    pthread_t reindeer_ids[REINDEER_TOTAL];
    for(int i=0;i<REINDEER_TOTAL;i++) {
        pthread_create(&reindeer_ids[i], NULL, reindeer_routine, NULL);
    }

    pthread_t elf_ids[ELVES_TOTAL];
    for (int i=0;i<ELVES_TOTAL;i++) {
        pthread_create(&elf_ids[i], NULL, elf_routine, NULL);
    }

    pthread_join(santa, NULL);
    return EXIT_SUCCESS;
}
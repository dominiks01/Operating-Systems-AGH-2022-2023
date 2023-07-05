#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct sembuf sembuf;

#define BARBERS 0
#define CHAIRS 1
#define WAITING_ROOM 2

int value;
int sem_set, shm_id, client_id = 0;
int *last_served;
key_t sem_key, shm_key;
sembuf operation[1];

void handle_exit() {
  printf("Konczenie symulacji!\n");
  shmdt(last_served);
  shmctl(shm_id, IPC_RMID, NULL);
  semctl(sem_set, 0, IPC_RMID);
  exit(EXIT_FAILURE);
}

int free_sem(int n) {
  operation[0].sem_op = 1;
  operation[0].sem_num = n;
  operation[0].sem_flg = 0;

  return semop(sem_set, operation, 1);
}

int acquire_sem(int n) {
  operation[0].sem_num = n;
  operation[0].sem_op = -1;
  operation[0].sem_flg = 0;

  return semop(sem_set, operation, 1);
}

int max(int num1, int num2) { return (num1 > num2) ? num1 : num2; }

void haircut() {
  printf("Klient [%d] jest obslugiwany!\n", client_id);
  int haircut_time = rand() % 2 + 3;
  sleep(haircut_time);
  printf("Klient [%d] obsluzony\n", client_id);
  *last_served = max(*last_served, client_id);
  free_sem(CHAIRS);
}

int main(int argc, char *argv[]) {

  if (argc < 4) {
    fprintf(stderr, "[ERROR] Too few arguments!\n");
    exit(EXIT_FAILURE);
  }

  key_t sem_key = ftok(getenv("HOME"), 1);
  key_t shm_key = ftok(getenv("HOME"), 1);

  if (sem_key == (key_t)-1) {
    fprintf(stderr, "[ERROR] ftok() ended with error! %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (shm_key == (key_t)-1) {
    fprintf(stderr, "[ERROR] ftok() ended with error! %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((sem_set = semget(sem_key, 3, IPC_CREAT | 0666)) == -1) {
    fprintf(stderr, "[ERROR] semget() ended with error! %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((shm_id = shmget(shm_key, sizeof(int), IPC_CREAT | 0666)) == -1) {
    fprintf(stderr, "[ERROR] shmget() ended with error! %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, handle_exit);
  signal(SIGQUIT, handle_exit);

  semctl(sem_set, BARBERS, SETVAL, atoi(argv[1]));
  semctl(sem_set, CHAIRS, SETVAL, atoi(argv[2]));
  semctl(sem_set, WAITING_ROOM, SETVAL, atoi(argv[3]));

  free_sem(BARBERS);
  free_sem(CHAIRS);
  free_sem(WAITING_ROOM);

  last_served = shmat(shm_id, NULL, 0);
  *last_served = 0;

  while (1) {
    client_id++;
    sleep(1);

    int barbers = semctl(sem_set, BARBERS, GETVAL);
    int chairs = semctl(sem_set, CHAIRS, GETVAL);
    int waiting_room = semctl(sem_set, WAITING_ROOM, GETVAL);

    printf("B:%d C:%d WR:%d\n", barbers, chairs, waiting_room);
    int pid = fork();
    if (pid == 0) {
      if (acquire_sem(BARBERS) != -1) {
        printf("Klient [%d] znalazl fryzjera\n", client_id);

        if (acquire_sem(CHAIRS) != -1) {
          haircut();
        } else {
          if (acquire_sem(WAITING_ROOM) != -1) {
            printf("Klient [%d] trafil do poczekalni\n", client_id);

            while (client_id <= *last_served)
              ;
            free_sem(WAITING_ROOM);
            haircut();
          } else {
            printf("Klient %d nie znalazl miejsca w poczekalni i wyszedl\n",
                   client_id);
            *last_served = max(*last_served, client_id);
          }
        }
        free_sem(BARBERS);
      } else {
        acquire_sem(WAITING_ROOM);
        while (client_id <= *last_served)
          ;
        free_sem(WAITING_ROOM);
        acquire_sem(CHAIRS);
        haircut();
      }
      exit(EXIT_SUCCESS);
    }

    if (pid == -1) {
      handle_exit();
      fprintf(stderr, "[ERROR] fork() stopped working %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
}
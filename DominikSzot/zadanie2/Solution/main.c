#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <err.h>

#define PROCESS_INTERUPTED 5

void handler(int sig){
  printf("Process: received signal [%d]\n", sig);
  exit(PROCESS_INTERUPTED);
}

int main(int argc, char ** argv) {

  pid_t child_pid = fork();
  int child_status;

  if(signal(SIGINT, handler) != 0){
    fprintf(stderr, "[ERROR] signal() %d\n", errno);
    exit(EXIT_FAILURE);
  }

  switch( child_pid ) {
   case 0:
     sleep(3);
     printf("Potomek: %d kończy działanie\n",getpid());
     exit(EXIT_SUCCESS);
   case -1:
     perror("FORK");
     exit (EXIT_FAILURE);
   default:
    {
     if (argc > 1) sleep(atoi(argv[1]));

      // Sending kill signal to process with child_pid ID
      if(kill(child_pid, SIGINT) == -1){
        fprintf(stderr, "[ERROR] kill(): %d\n", errno);
        exit(EXIT_FAILURE);
      }

      if(waitpid(child_pid, &child_status, 0) == -1){
        fprintf(stderr, "[ERROR] watipid(): %d\n", errno);
        exit(EXIT_FAILURE);
      }

      int process_status = WEXITSTATUS(child_status);

      if(process_status == EXIT_SUCCESS){
        printf("Process ended normally! PID: [%d] STATUS: [%d] \n", child_pid, child_status);
      }

      if(process_status != EXIT_SUCCESS){
        printf("Proces ended with signal SIGNAL VALUE: [%d]\n", WEXITSTATUS(child_status));
      }


  /* Wyślij do potomka sygnał SIGINT i obsłuż błąd */
  /* Czekaj na zakończenie potomka i pobierz status.
     1) Jeśli potomek zakończył się normalnie (przez exit), wypisz informację wraz wartością statusu i jego PID.
     2) Jeśli potomek zakończył się sygnałem, zwróć taką informację (wartość sygnału) */

	 
	}
 }
  return 1;
}











   /* koniec */
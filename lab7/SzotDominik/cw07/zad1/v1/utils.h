#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>

char *attach_shared_memory(const char* filename, int size);
bool detach_shared_memory(char *shared_memory);
bool destroy_shared_memory(const char *filename);

typedef int Semaphore;

Semaphore create_semaphore(const char *filename, int initial);
Semaphore open_semaphore(const char *filename);
void close_semaphore(Semaphore sem);
void unlink_semaphore(const char* filename);
void aquire(Semaphore sem);
void release(Semaphore sem);

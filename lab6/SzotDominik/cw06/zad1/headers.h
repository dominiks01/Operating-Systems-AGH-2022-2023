#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define LIST    1   
#define _2ALL   2
#define _2ONE   3
#define STOP    4
#define INNIT   5

#define SERVER_KEY_PATHNAME "/tmp/queue_path"
#define PROJECT_ID 'M'

#define SUCCES 10
#define NEW_MESSAGE 11 
#define FAILURE 12
#define IGNORE 13

#define MAX_BUFF_SIZE 255

struct message {
    long message_type;
    char message_buffer[MAX_BUFF_SIZE];
    key_t key; 
    int client_id;
    int other_id;
    time_t message_time;    
};
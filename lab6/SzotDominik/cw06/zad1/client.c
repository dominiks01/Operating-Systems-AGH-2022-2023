#include <asm-generic/errno.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include "headers.h"

#define SIGNAL SIGUSR1
#define BUFFER_SIZE 1024

int server_qid, myqid, other_id;
struct message my_message, return_message; 
key_t server_queue_key;
time_t request_time;

struct command{
    char *str;
    int n; 
};

struct command dict[] = {
    {"2ALL",   _2ALL},
    {"2ONE",   _2ONE},
    {"LIST",   LIST}, 
    {"STOP",   STOP},
};

int compare(const void *s1, const void *s2){
    const struct command *e1  = s1; 
    const struct command *e2  = s2;
    return strcmp(e1->str, e2->str); 
}

struct command* map_command_to_int(char * arg){
    struct command *result, key = {arg};
    result = bsearch(&key, dict, sizeof(dict)/sizeof(dict[0]),
                sizeof dict[0], compare);
    return result;
}

void end_work(){
    printf("Client stop working!\n");
    msgctl(myqid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void handle_message(struct message message){
    switch (message.message_type) {
        case SUCCES:
            break;
        
        case NEW_MESSAGE:
            printf("received new message: \n%s\n", message.message_buffer);
            break;

        case STOP:
            printf("client stopping work!\n");
            end_work();
            break;

        case FAILURE:
            printf("connection failed!\n");
            end_work();

        case IGNORE:    
            default:
            break;
        }
    
    return;
}

void handle_stop_command(){
    my_message.message_type = STOP;
    sprintf(my_message.message_buffer, "Stop command!");

    msgsnd (server_qid, &my_message, sizeof (struct message), IPC_NOWAIT);
    end_work();
    return;
}

void handle_stop_signal(int singnal_l){
    handle_stop_command();
    exit(EXIT_SUCCESS);
}

void modify_message(int type, char* text, long int other_id, time_t time){
    my_message.message_type = type;
    sprintf(my_message.message_buffer, "%s", text);
    my_message.other_id = other_id;
    my_message.message_time = time;
}   

int main (int argc, char **argv){
    char line[MAX_BUFF_SIZE];

    signal(SIGSTOP, handle_stop_signal);
    signal(SIGINT, handle_stop_signal);

    qsort(dict, sizeof(dict)/sizeof(dict[0]), sizeof dict[0], compare);

    if (!(myqid = msgget (IPC_PRIVATE, 0660))) {
        fprintf(stderr,"COULD NOT CREATE CLIENT QUEUE! ERROR: %s\n", strerror( errno ));
        exit (EXIT_FAILURE);
    }

    if(!(server_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID))){
        fprintf(stderr, "ftok() ENDED WITH ERROR: %s\n", strerror( errno ));
        end_work();
    }

    if ((server_qid = msgget(server_queue_key, 0)) == -1) {
        fprintf(stderr,"MSGGET ERROR: %s\n", strerror( errno ));
        end_work();
    }

    modify_message(INNIT, "Innit command", -1, time(NULL));
    my_message.key = myqid;

    if (msgsnd (server_qid, &my_message, sizeof (struct message), 0) == -1) {
        fprintf(stderr,"CLIENT MSGSND ERROR: %s\n", strerror( errno ));
        end_work();
    } 

    if(msgrcv (myqid, &return_message, sizeof (struct message), 0, 0) == -1) {
        fprintf(stderr,"Could not connect to server!\n");
        end_work();
    } 

    handle_message(return_message);
    my_message.client_id = return_message.client_id;  
    
    while (1) {
        scanf("%s", line);
        struct command *result = map_command_to_int(line);

        if(result == NULL){
            fprintf(stderr,"Unknown command!\n");
            continue;
        } 

        while (msgrcv (myqid, &return_message, sizeof (struct message), 0, IPC_NOWAIT) >= 0) {
            handle_message(return_message);
        }

        switch (result->n) {
            case LIST: {
                modify_message(LIST, "LIST REQUEST", -1, time(NULL));

                if (msgsnd (server_qid, &my_message, sizeof (struct message), 0) == -1) {
                    fprintf(stderr,"CLIENT MSGSND ERROR: %s\n", strerror( errno ));
                    end_work();
                }

                if(msgrcv(myqid, &return_message, sizeof (struct message), NEW_MESSAGE, 0) == -1){
                    fprintf(stderr, "No response from server!\n");
                    end_work();
                } else {
                    handle_message(return_message);
                }
                
                break;
            }
            case _2ALL: {
                printf("CC: ");
                scanf(" %[^\n]", line); 
                modify_message(_2ALL, line, -1, time(NULL));

                if (msgsnd (server_qid, &my_message, sizeof (struct message), 0) == -1) {
                    fprintf(stderr,"CLIENT MSGSND ERROR: %s\n", strerror( errno ));
                    end_work();
                }
                break;
            }

            case _2ONE: {
                printf("Enter ID && Message\n");
                scanf(" %d %[^\n]", &other_id, line);
                modify_message(_2ONE, line, other_id, time(NULL));

                if (msgsnd (server_qid, &my_message, sizeof (struct message), 0) == -1) {
                    fprintf(stderr,"CLIENT MSGSND ERROR: %s\n", strerror( errno ));
                    end_work();
                }
                break;
            }
            case STOP: {
                handle_stop_command();
                exit(EXIT_SUCCESS);
            }

            default:
                printf("UNKNOWN COMMAND\n");
                continue;
        }
    }
    exit (0);
}
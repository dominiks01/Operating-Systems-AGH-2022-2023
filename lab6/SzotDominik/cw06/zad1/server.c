#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "headers.h"

#define QUEUE_PERMISSIONS 0666
#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024
#define SIGNAL SIGUSR1

extern int errno;

FILE *fp;
char log_line[BUFFER_SIZE];
key_t queues[MAX_CLIENTS];
int no_clients_actual_connected = 0, qid;;
struct message message;

void display_message(struct message message){
    printf("TYPE [%ld]\n", message.message_type);
    printf("TEXT [%s]\n", message.message_buffer);
    printf("KEY [%d]\n", message.key);
    printf("CLIENT ID [%d]\n", message.client_id);
    printf("OTHER ID [%d]\n", message.other_id);

    struct tm tm = *localtime(&(message.message_time));
    printf("time: %d-%02d-%02d %02d:%02d:%02d\n\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    return;
}

void handle_new_connection(){    
    if(no_clients_actual_connected == MAX_CLIENTS){
        message.message_type = FAILURE;
        msgsnd (message.key, &message, sizeof (struct message), IPC_NOWAIT);
        return;    
    }

    snprintf(message.message_buffer, MAX_BUFF_SIZE, "SUCCED!");
    printf("NEW CLIENT ADDED\n");
    message.message_type = SUCCES;
    no_clients_actual_connected++;

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(queues[i] == -1){
            queues[i] = message.key;
            message.client_id = i;
            break;
        }
    }

    if (msgsnd (message.key, &message, sizeof (struct message), IPC_NOWAIT) == -1) {  
        perror ("msgget");
        return;
    } 

    struct tm tm = *localtime(&(message.message_time));
    size_t size = snprintf(log_line, 1024, "INNIT COMMAND\nKEY[%d]\ntime:%d-%02d-%02d %02d:%02d:%02d\n\n", message.key, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    size_t _ = fwrite(log_line, sizeof(char), size, fp);

    return;
}

void handle_send_to_all(struct message message){
    message.message_type = NEW_MESSAGE;
    printf("SENDING MESSAGE\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(queues[i] != -1 && i != message.client_id){
            if (msgsnd (queues[i], &message, sizeof (struct message), 0) == -1) {  
                perror ("msgget");
                exit (1);
            }
        }
    }

    struct tm tm = *localtime(&(message.message_time));
    size_t size = snprintf(log_line, 1024, "SEND TO ALL\nFROM[%d]\ntime:%d-%02d-%02d %02d:%02d:%02d\n\n", message.client_id, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    size_t _ = fwrite(log_line, sizeof(char), size, fp);

    return;
}

void handle_send_to_one(struct message message){
    message.message_type = NEW_MESSAGE;

    if(queues[message.other_id] == -1){
        fprintf(stderr, "NO SUCH USER\n");
        return;
    }

    if (msgsnd (queues[message.other_id], &message, sizeof (struct message), 0) == -1) {  
        perror ("msgget");
        return;
    }

    struct tm tm = *localtime(&(message.message_time));
    size_t size = snprintf(log_line, 1024, "SEND TO ONE\nFROM[%d] TO[%d]\ntime:%d-%02d-%02d %02d:%02d:%02d\n\n", message.client_id,message.other_id, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    size_t _ = fwrite(log_line, sizeof(char), size, fp);

    return;
}


void handle_list_command(struct message message){
    char buffer[MAX_BUFF_SIZE] = { 0 };
    char fmtbuf[MAX_BUFF_SIZE];

    strcat(buffer, "Index of clients running:\n");

    int i;
    for(i = 0; i < MAX_CLIENTS; i++){
        if(queues[i] != -1){
            snprintf(fmtbuf,MAX_BUFF_SIZE,"[%d][%d]\n",i, queues[i]);
            strcat(buffer,fmtbuf);
        }
    }

    message.message_type = NEW_MESSAGE;
    snprintf(message.message_buffer, MAX_BUFF_SIZE, "%s", buffer);

    if (msgsnd (message.key, &message, sizeof (struct message), 1) == -1) {  
        perror ("msgget");
        exit (1);
    }

    struct tm tm = *localtime(&(message.message_time));
    size_t size = snprintf(log_line, 1024, "LIST COMMAND \nFROM USER[%d]\ntime:%d-%02d-%02d %02d:%02d:%02d\n\n", message.client_id, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    size_t _ = fwrite(log_line, sizeof(char), size, fp); 

    printf("MESSAGE SENT!\n");
    return;
}

void handle_stop(struct message message){
    no_clients_actual_connected -= 1;

    for(int i = 0; i < MAX_CLIENTS; i++)
        if(queues[i] == message.key)
            queues[i] = -1;

    struct tm tm = *localtime(&(message.message_time));
    size_t size = snprintf(log_line, 1024, "STOP \nFROM[%d]\ntime:%d-%02d-%02d %02d:%02d:%02d\n\n", message.client_id, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    size_t _ = fwrite(log_line, sizeof(char), size, fp);    

    return;
}

void handle_stop_signal(int singnal_l){
    message.message_type = STOP;
    printf("STOPPING SERVER WORK!\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(queues[i] != -1){
            if (msgsnd (queues[i], &message, sizeof (struct message), 1) == -1) {  
                perror ("msgget");
                exit (1);
            }
            queues[i] = -1;
        }
    }

    msgctl(qid, IPC_RMID, NULL);
    fclose(fp);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
    fp = fopen("LOG.txt", "ab+");
    time_t t= time(NULL);
    struct tm tm = *localtime(&(t));

    size_t size = snprintf(log_line, 1024, "Server start\ntime:%d-%02d-%02d %02d:%02d:%02d\n\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    size_t _ = fwrite(log_line, sizeof(char), size, fp);
   
    key_t new_key;

    signal(SIGSTOP, handle_stop_signal);
    signal(SIGINT, handle_stop_signal);

    for(int i = 0; i < MAX_CLIENTS; i++)
        queues[i] = -1;

    if((new_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID))==-1){
        fprintf(stderr, "ftok() ended with ERROR! %s\n", strerror( errno ));
        exit(EXIT_FAILURE);
    }

    if((qid = msgget(new_key, IPC_CREAT | 0666))==-1){
        fprintf(stderr, "Cannot create new msgque! %s\n", strerror( errno ));
        exit(EXIT_FAILURE);
    }

    printf ("Server: Running!\n");

    while(1){
        if(msgrcv (qid, &message, sizeof (struct message), 0, IPC_NOWAIT) < 0) {
            continue;
        }

        switch (message.message_type) {
            case INNIT: {
                handle_new_connection(message);
                break;
            }

            case LIST: {
                handle_list_command(message);
                break;
            }

            case _2ALL:{
                handle_send_to_all(message);
                break;
            }

            case _2ONE:{
                handle_send_to_one(message);
                break;
            }

            case STOP:{
                handle_stop(message);
                break;
            }

            default: {
                printf("NONONONO");
                break;
            }
        }
    }
    


    return 0;
}

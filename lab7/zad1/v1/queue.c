#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "common.h"

char queue_pop(char *queue) {
    if(queue_empty(queue)) {
        fprintf(stderr, "Queue empty.\n");
        return '\0';
    }
    char byte = queue[0];
    memcpy(queue, queue + 1, strlen(queue) + 1);
    return byte;
}

void queue_push(char *queue, char byte) {
    if(queue_full(queue)) {
        fprintf(stderr, "Queue full.\n");
        return;
    }
    int size = strlen(queue);
    queue[size] = byte;
    queue[size + 1] = '\0';
}

int queue_full(char *queue) {
    return (strlen(queue) + 1 == BUFFER_SIZE);
}

int queue_empty(char *queue) {
    return (strlen(queue) == 0);
}
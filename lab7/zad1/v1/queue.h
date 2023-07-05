#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

char queue_pop(char *queue);
void queue_push(char *queue, char byte);
int queue_full(char *queue);
int queue_empty(char *queue);

#endif
#include "stm32l4xx.h"

#define QUEUE_SIZE 32

#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue {
  int head;
  int tail;
  uint8_t buffer[QUEUE_SIZE];
} queue_t;

#endif


void init_queue(queue_t *);
int enqueue(queue_t *, uint8_t );
uint8_t dequeue(queue_t *);
int queue_empty(queue_t *);

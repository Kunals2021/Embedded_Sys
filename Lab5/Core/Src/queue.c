#include "queue.h"
#include "interrupt.h"

void init_queue(queue_t *buf) {
  uint32_t mask;
  mask = disable();
  buf->head=0;
  buf->tail=0;
  restore(mask);
}

int enqueue (queue_t *buf, uint8_t data) {
  int return_val=0;
  uint32_t mask;
  mask = disable();
  if ((((buf->head)+1)%QUEUE_SIZE)!=buf->tail) {
    buf->buffer[buf->head]=data;
    buf->head=((buf->head)+1)%QUEUE_SIZE;
    return_val=0;
  }
  else {
    return_val=-1;
  }
  restore(mask);
  return (return_val);
}

uint8_t dequeue (queue_t *buf) {
  int return_val=0;
  uint32_t mask;
  mask = disable();
  if (buf->tail!=buf->head) {
    return_val=buf->buffer[buf->tail];
    buf->tail=((buf->tail)+1)%QUEUE_SIZE;
  }
  restore(mask);
  return(return_val);
}

int queue_empty(queue_t *buf) {
  int retval = 0;
  uint32_t mask;
  mask = disable();
  if (buf->head==buf->tail) {
    retval=1;
  }
  restore(mask);
  return (retval);
}

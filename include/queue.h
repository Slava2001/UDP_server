#ifndef INCLUDE_QUEUE_H
#define INCLUDE_QUEUE_H

#include "tlv_parser.h"

#define MAX_QUEUE_LEN 10

struct queue {
    command_t buff[MAX_QUEUE_LEN];
    uint8_t ptr;
    uint8_t cnt;
};

int queue_put_element(struct queue *q, command_t *c);
int queue_get_element(struct queue *q, command_t *c);

#endif // INCLUDE_QUEUE_H
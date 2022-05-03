#include "queue.h"
#include "util.h"

int queue_put_element(struct queue *q, command_t *c) {
    uint8_t tail = (q->ptr+q->cnt) % MAX_QUEUE_LEN;
    q->buff[tail] = *c;
    q->cnt++;
    if (q->cnt > MAX_QUEUE_LEN) {
        q->ptr = (q->ptr + 1) % MAX_QUEUE_LEN; // drop first element
        q->cnt--;
        log_warn("element has been dropped");
        return -1;
    }
    return 0;
}

int queue_get_element(struct queue *q, command_t *c) {
    if (q->cnt <= 0) {
        return -1; // queue is empty
    }
    q->cnt--;
    *c = q->buff[q->ptr];
    q->ptr = (q->ptr + 1) % MAX_QUEUE_LEN;
    return 0;
}

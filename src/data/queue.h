#ifndef _DATA_QUEUE_H_
#define _DATA_QUEUE_H_

#include <inttypes.h>
#include <stddef.h>

typedef struct {
    size_t capacity, size, len;
    uint32_t index;
    void *data;
} queue_t;

void queue_init(queue_t *, size_t capacity);
void queue_destroy(queue_t *);

void *queue_peak(const queue_t *);

void *queue_pop(queue_t *);
void queue_push(queue_t *, void *data);

#endif
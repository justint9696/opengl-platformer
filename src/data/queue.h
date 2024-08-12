#ifndef _DATA_QUEUE_H_
#define _DATA_QUEUE_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t capacity, size, len;
    uint32_t index;
    void *data;
} queue_t;

/* Allocates a block of memory for a queue. */
void queue_init(queue_t *, size_t size, size_t capacity);

/* Releases the allocated block of memory. */
void queue_destroy(queue_t *);

/* Returns the first item of a queue. */
void *queue_peak(const queue_t *);

/* Returns the last item of a queue. */
void *queue_rear(const queue_t *);

/* Removes the next queued item and returns it. */
void *queue_pop(queue_t *);

/* Pushes an item at the end of a queue. */
void queue_push(queue_t *, const void *data);

/* Removes all items from a queue. */
void queue_clear(queue_t *);

#endif // ifndef _DATA_QUEUE_H_

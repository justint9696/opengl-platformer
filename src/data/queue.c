#include "data/queue.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void queue_init(queue_t *self, size_t size, size_t capacity) {
    memset(self, 0, sizeof(queue_t));

    self->size = size;
    self->capacity = capacity;

    self->data = calloc(capacity, self->size);
    assert(self->data);
}

void queue_destroy(queue_t *self) {
    assert(self->data);
    free(self->data);
}

void *queue_peak(const queue_t *self) {
    return (self->data + (self->size * self->index));
}

void *queue_pop(queue_t *self) {
    void *item = queue_peak(self);
    assert(item);
    self->index = (self->index + 1) % self->capacity;
    --self->len;
    return item;
}

void queue_push(queue_t *self, void *data) {
    assert(self->len < self->capacity);
    void *ptr = (self->data + (self->size * (self->index + self->len++)));
    memcpy(ptr, data, self->size);
}

void queue_clear(queue_t *self) {
    self->len = 0;
    self->index = 0;
}

/**
 * @file array.c
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Dynamic array implementation functions.
 */

#include "data/array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void *array_init(void *ptr, size_t size, size_t capacity) {
    assert(ptr);

    array_t *self = (array_t *)ptr;
    memset(self, 0, sizeof(array_t) + (size * capacity));

    self->capacity = capacity;
    self->size = size;

    return &self->data;
}

void *array_alloc(size_t size, size_t capacity) {
    array_t *self = calloc(1, sizeof(array_t) + (size * capacity));
    assert(self);

    self->size = size;
    self->capacity = capacity;
    self->dynamic = true;

    return &self->data;
}

void array_free(void *ptr) {
    assert(ptr);

    array_t *self = array_header(ptr);
    free(self);
}

void *array_resize(void *ptr, size_t capacity) {
    assert(ptr);

    array_t *self = array_header(ptr);
    assert(capacity >= self->len);

    self = realloc(self, self->size * capacity);
    assert(self);

    self->capacity = capacity;

    return &self->data;
}

int _array_push_intern(void **dataptr, const void *item) {
    void *ptr = *dataptr;

    assert(ptr);
    array_t *self = array_header(ptr);

    assert((self->len < self->capacity) || (self->dynamic));
    if (self->len >= self->capacity - 1) {
        self->capacity *= 2;
        self = realloc(self, sizeof(array_t) + (self->size * self->capacity));
        *dataptr = &self->data;
    }

    void *dst = ((void *)&self->data + (self->size * self->len));
    memcpy(dst, item, self->size);

    return self->len++;
}

int array_remove(void *ptr, const void *item) {
    assert(ptr);
    array_t *self = array_header(ptr);

    void *dst;
    size_t idx;
    for (idx = 0; idx < self->len; idx++) {
        dst = array_at(ptr, idx);
        if (!memcmp(dst, item, self->size))
            break;
    }

    if (idx >= self->len)
        return -1;

    // check if the item is at the last index of the array
    if (idx < self->len - 1) {
        void *src = array_at(ptr, idx + 1);
        memcpy(dst, src, self->size * (self->len - idx - 1));
    }

    return --self->len;
}

void array_clear(void *data) {
    assert(data);

    array_t *self = array_header(data);
    memset(&self->data, 0, self->size * self->capacity);
    self->len = 0;
}

void *array_at(void *ptr, uint32_t index) {
    assert(ptr);

    array_t *self = array_header(ptr);
    if (index > self->len)
        return NULL;

    return (void *)(ptr + (self->size * index));
}

size_t array_len(void *ptr) {
    assert(ptr);

    array_t *self = array_header(ptr);
    return self->len;
}

void *array_slice(void *ptr, uint32_t start, uint32_t end) {
    assert(ptr);

    array_t *self = array_header(ptr);
    void *slice = array_alloc(self->size, self->capacity);
    memcpy(slice, array_at(self, start), (end - start) * self->size);

    self = array_header(slice);
    self->len = (end - start);

    return slice;
}

void array_copy(void *dst, void *src) {
    assert(dst);
    array_t *a = array_header(dst);

    assert(src);
    array_t *b = array_header(src);

    assert(a->capacity >= b->capacity);
    memcpy(&a->data, &b->data, b->size * b->len);
}

void array_insert(void *ptr, uint32_t index, const void *item) {
    assert(ptr);

    array_t *self = array_header(ptr);
    assert(index < self->capacity);

    if (index >= self->len) {
        array_push(ptr, item);
        return;
    }

    void *src = array_at(ptr, index);
    void *dst = (((void *)src) + self->size);
    assert(self->len < self->capacity);

    memcpy(dst, src, (self->len - index) * self->size);
    memcpy(src, item, self->size);

    self->len++;
}

bool array_contains(void *ptr, const void *item) {
    assert(ptr);

    array_t *self = array_header(ptr);
    for (size_t i = 0; i < self->len; i++) {
        void *dst = array_at(ptr, i);
        if (!memcmp(dst, item, self->size))
            return true;
    }

    return false;
}

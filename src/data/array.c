#include "data/array.h"
#include "util/io.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t capacity, size, len;
    void *data;
} array_t;

#define array_header(_a) ((array_t *)(((void *)(_a)) - offsetof(array_t, data)))

void *array_alloc(size_t size, size_t capacity) {
    array_t *array = calloc(1, sizeof(array_t) + (size * capacity));
    array->capacity = capacity;
    array->size = size;
    return (void *)&array->data;
}

void array_free(void *data) {
    array_t *array = array_header(data);
    assert(array);
    free(array);
}

void array_resize(void **dataptr, size_t capacity) {
    array_t *array = array_header(*dataptr);
    assert(array && capacity >= array->len);
    array->capacity = capacity;
    array = realloc(array, array->size * capacity);
    *dataptr = (void *)&array->data;
}

uint32_t array_push(void **dataptr, void *item) {
    array_t *array = array_header(*dataptr);
    assert(array);
    if (array->len >= array->capacity) {
        array_resize(dataptr, array->capacity * 2);
    }
    void *ptr = array_get(*dataptr, array->len);
    memcpy(ptr, item, array->size);
    return array->len++;
}

void array_remove(void *data, void *item) {
    array_t *array = array_header(data);
    assert(array);

    void *dst;
    size_t idx;
    for (idx = 0; idx < array->len; idx++) {
        dst = array_get(data, idx);
        if (!memcmp(dst, item, array->size))
            break;
    }

    if (idx >= array->len) {
        WARN(0, "Attempted to remove an item not contained within the array.\n");
        return;
    }

    // check if the item is at the last index of the array
    if (idx < array->len - 1) {
        void *src = array_get(data, idx + 1);
        memcpy(dst, src, array->size * (array->len - idx - 1));
    }

    array->len--;
}

void array_clear(void *data) {
    array_t *array = array_header(data);
    assert(array);
    memset(data, 0, array->size * array->capacity);
    array->len = 0;
}

void *array_get(void *data, uint32_t index) {
    array_t *array = array_header(data);
    assert(array);
    return (void *)(data + (array->size * index));
}

size_t array_len(void *data) {
    array_t *array = array_header(data);
    assert(array);
    return array->len;
}

void *array_slice(void *data, uint32_t start, uint32_t end) {
    array_t *array = array_header(data);
    assert(array);
    void *slice = array_alloc(array->size, array->capacity);
    memcpy(slice, array_get(array, start), (end - start) * array->size);
    array_header(slice)->len = (end - start);
    return slice;
}

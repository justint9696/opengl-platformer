#include "data/array.h"

#include "util/log.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t capacity, size, len;
    void *userdata;
} array_t;

#define array_header(_a)\
    ((array_t *)(((void *)(_a)) - offsetof(array_t, userdata)))

void *array_init(void *data, size_t size, size_t capacity) {
    assert(data);

    array_t *array = (array_t *)data;
    /* memset(array, 0, sizeof(array_t) * (size * capacity)); */

    array->capacity = capacity;
    array->size = size;

    return &array->userdata;
}

void *array_alloc(size_t size, size_t capacity) {
    array_t *array = calloc(1, sizeof(array_t) + (size * capacity));
    assert(array);

    array->capacity = capacity;
    array->size = size;

    return &array->userdata;
}

void array_free(void *data) {
    assert(data);
    array_t *array = array_header(data);
    free(array);
}

void array_resize(void **dataptr, size_t capacity) {
    assert(*dataptr);

    array_t *array = array_header(*dataptr);
    assert(capacity >= array->len);

    array->capacity = capacity;
    array = realloc(array, array->size * capacity);

    *dataptr = &array->userdata;
}

uint32_t array_push(void *data, void *item) {
    assert(data);
    array_t *array = array_header(data);

    assert(array->len < array->capacity);
    void *ptr = array_get(data, array->len);
    memcpy(ptr, item, array->size);

    return array->len++;
}

void array_remove(void *data, void *item) {
    assert(data);
    array_t *array = array_header(data);

    void *dst;
    size_t idx;
    for (idx = 0; idx < array->len; idx++) {
        dst = array_get(data, idx);
        if (!memcmp(dst, item, array->size))
            break;
    }

    if (idx >= array->len) {
        log_warn( 
            "Attempted to remove an item not contained within the array.\n");
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
    assert(data);
    array_t *array = array_header(data);
    memset(data, 0, array->size * array->capacity);
    array->len = 0;
}

void *array_get(void *data, uint32_t index) {
    assert(data);

    array_t *array = array_header(data);
    return (void *)(data + (array->size * index));
}

size_t array_len(void *data) {
    assert(data);

    array_t *array = array_header(data);
    return array->len;
}

void *array_slice(void *data, uint32_t start, uint32_t end) {
    assert(data);

    array_t *array = array_header(data);
    void *slice = array_alloc(array->size, array->capacity);
    memcpy(slice, array_get(array, start), (end - start) * array->size);

    array = array_header(slice);
    array->len = (end - start);

    return slice;
}

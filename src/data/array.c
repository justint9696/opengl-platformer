#include "data/array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void *array_init(void *ptr, size_t size, size_t capacity) {
    assert(ptr);

    array_t *array = (array_t *)ptr;
    memset(array, 0, size * capacity);

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

int array_push(void *data, const void *item) {
    assert(data);
    array_t *array = array_header(data);

    if (array->len > array->capacity)
        return -1;

    void *ptr = array_get(data, array->len);
    memcpy(ptr, item, array->size);

    return array->len++;
}

int array_remove(void *data, const void *item) {
    assert(data);
    array_t *array = array_header(data);

    void *dst;
    size_t idx;
    for (idx = 0; idx < array->len; idx++) {
        dst = array_get(data, idx);
        if (!memcmp(dst, item, array->size))
            break;
    }

    if (idx >= array->len)
        return -1;

    // check if the item is at the last index of the array
    if (idx < array->len - 1) {
        void *src = array_get(data, idx + 1);
        memcpy(dst, src, array->size * (array->len - idx - 1));
    }

    array->len--;

    return 0;
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
    if (index > array->len)
        return NULL;

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

void array_copy(void *dst, void *src) {
    assert(dst);
    array_t *a = array_header(dst);

    assert(src);
    array_t *b = array_header(src);

    assert(a->capacity >= b->capacity);
    memcpy(&a->userdata, &b->userdata, b->size * b->len);
}

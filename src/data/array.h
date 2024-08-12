#ifndef _DATA_ARRAY_H_
#define _DATA_ARRAY_H_

#include <stddef.h>
#include <stdint.h>

/* For the internal headers of arrays. */
typedef struct {
    size_t capacity, size, len;
    void *userdata;
} array_t;

#define array_header(_a)\
    ((array_t *)(((void *)(_a)) - offsetof(array_t, userdata)))

/* Initializes an array at the given memory address. */
void *array_init(void *ptr, size_t size, size_t capacity);

/* Allocates a block of memory and returns a pointer to its data section. */
void *array_alloc(size_t size, size_t capacity);

/* Releases the allocated block of memory. */
void array_free(void *data);

/* Removes all items from an array. */
void array_clear(void *data);

/* Reallocates the provided block of memory with provided capacity. */
void array_resize(void **dataptr, size_t capacity);

/* 
 * Pushes an item to the end of an array.
 * Returns -1 on failure or the index of the item on success.
 */
int array_push(void *data, const void *item);

/*
 * Removes an item from an array.
 * Returns -1 on failure or 0 on success. 
 */
int array_remove(void *data, const void *item);

/*
 * Returns a pointer to the item at the provided index.
 * Returns NULL if index is invalid.
 */
void *array_get(void *data, uint32_t index);

/* Returns the number of items in an array. */
size_t array_len(void *data);

/*
 * Allocates a block of memory and copies the items from indices [start:end].
 * Returns a pointer to the data section of allocated array.
 */
void *array_slice(void *data, uint32_t start, uint32_t end);

/* Copies the contents from the source array to the destination array. */
void array_copy(void *dst, void *src);

#endif // ifndef _DATA_ARRAY_H_

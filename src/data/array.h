#ifndef _DATA_ARRAY_H_
#define _DATA_ARRAY_H_

#include <stddef.h>
#include <stdint.h>

// initializes an array at the given memory address
void *array_init(void *ptr, size_t size, size_t capacity);

void *array_alloc(size_t size, size_t capacity);
void array_free(void *data);

void array_clear(void *data);
void array_resize(void **dataptr, size_t capacity);

uint32_t array_push(void *data, void *item);
void array_remove(void *data, void *item);

void *array_get(void *data, uint32_t index);
size_t array_len(void *data);

void *array_slice(void *data, uint32_t start, uint32_t end);
void array_copy(void *dst, void *src);

#endif // _DATA_ARRAY_H_

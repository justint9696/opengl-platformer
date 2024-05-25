#ifndef _DATA_ARRAY_H_
#define _DATA_ARRAY_H_

#include <inttypes.h>
#include <stddef.h>

void *array_alloc(size_t size, size_t capacity);
void array_free(void *);

void array_clear(void *);
void array_resize(void **, size_t capacity);
uint32_t array_push(void **, void *item);
void array_remove(void *, void *item);
void *array_get(void *, uint32_t index);
size_t array_len(void *);
void *array_slice(void *, uint32_t start, uint32_t end);

#endif

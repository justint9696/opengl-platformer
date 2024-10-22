/**
 * @file allocator.c
 * @author Justin Tonkinson
 * @data 2024/10/18
 * @brief This file contains the implementation functions required for the
 * custom allocator.
 * */

#include "data/allocator.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/** @brief The maximum intended number of blocks. */
#define HEADER_MAX 32

/** @brief Returns the header of a free list. */
#define list_header(_ptr) \
    ((flist_t *)((char *)(_ptr) - offsetof(flist_t, data)))

void allocator_alloc(allocator_t *self, size_t n) {
    size_t size = n + (offsetof(flist_t, data) * HEADER_MAX);

    self->data = malloc(size);
    assert(self->data);

    self->capacity = size;

    self->head = self->data;
    self->head->size = size - offsetof(flist_t, data);
    self->head->next = NULL;

    assert(self->data == list_header(&self->head->data));
}

void allocator_free(allocator_t *self) {
    assert(self->data);
    free(self->data);
}

/**
 * @brief Initializes a block of memory.
 * @returns a pointer to the next header if there is space, or NULL otherwise
 */
static void *preprare(flist_t *tmp, size_t n) {
    assert(tmp);

    size_t size = n + offsetof(flist_t, data);
    if (tmp->size <= size)
        return NULL;

    flist_t *node = ((flist_t *)((char *)&tmp->data + n));
    node->size = (tmp->size - size);
    node->next = tmp->next;

    tmp->size = n;
    tmp->next = NULL;

    return node;
}

void *allocator_request(allocator_t *self, size_t n) {
    for (flist_t *tmp = self->head, *prev = NULL; tmp;
         prev = tmp, tmp = tmp->next) {
        if (tmp->size >= n) {
            void *ptr = preprare(tmp, n);

            if (prev) {
                prev->next = ptr;
            } else {
                self->head = ptr;
            }

            return &tmp->data;
        }
    }
    return NULL;
}

static void consolidate(flist_t *a, flist_t *b) {
    assert(a);
    assert(b);

    a->size += b->size + offsetof(flist_t, data);
    a->next = b->next;
}

static inline bool contiguous(flist_t *a, flist_t *b) {
    return ((flist_t *)((char *)&a->data + a->size) == b);
}

static void defrag(flist_t *prev, flist_t *next, flist_t *node) {
    assert(node);

    if (next && contiguous(node, next)) {
        consolidate(node, next);
    }

    if (prev && contiguous(prev, node)) {
        consolidate(prev, node);
    }
}

void allocator_release(allocator_t *self, void *ptr) {
    void *tail = ((char *)self->data + self->capacity);
    assert(ptr > self->data && ptr < tail);

    flist_t *prev = NULL;
    flist_t *tmp = self->head;
    for (; tmp; prev = tmp, tmp = tmp->next) {
        if ((((char *)tmp - (char *)ptr)) ||
            (prev && ((char *)prev - (char *)ptr)))
            break;
    }

    flist_t *node = list_header(ptr);
    llist_insert(self, prev, tmp, node);
    defrag(prev, tmp, node);
}

/**
 * @file allocator.h
 * @author Justin Tonkinson
 * @data 2024/10/18
 * @brief This file contains structures and function prototypes for a custom
 * allocator.
 */

#ifndef _DATA_ALLOCATOR_H__
#define _DATA_ALLOCATOR_H__

#include "data/linked_list.h"

#include <stddef.h>

typedef struct flist_s {
    /** @brief The byte size of the current block. */
    size_t size;

    /**
     * @brief Linked list node that points to the next item in the allocator. */
    LNODE(struct flist_s);

    /** @brief The data section of the node. */
    void *data;
} flist_t;

typedef struct allocator_s {
    /** @brief The total byte size of the allocated block. */
    size_t capacity;

    /** @brief Linked list node that points to the free blocks of memory. */
    LLIST(struct flist_s);

    /** @brief The data section of the allocator. */
    void *data;
} allocator_t;

/**
 * @brief Allocates a block of memory for an allocator.
 * @param self a pointer to an allocator
 * @param n number of bytes to allocate
 */
void allocator_alloc(allocator_t *, size_t n);

/**
 * @brief Releases the memeory allocated for an allocator.
 * @param self a pointer to an allocator
 */
void allocator_free(allocator_t *);

/**
 * @brief Returns a block of memory that can hold @a count number of items.
 * If there is not a block of memory available, the program will log the error
 * and terminate the program.
 * @param self a pointer to an allocator
 * @param n the number of bytes being requested
 * @returns a pointer to a block of memory if available, or NULL otherwise
 */
void *allocator_request(allocator_t *, size_t n);

/**
 * @brief Releases a block of memory from an allocator.
 * The block of memory must be a pointer that was requested from the allocator.
 * @see flist_request
 * @param self a pointer to an allocator
 * @param data a pointer to the block of memory being freed
 */
void allocator_release(allocator_t *, void *ptr);

#endif // ifndef _DATA_ALLOCATOR_H__

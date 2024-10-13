/**
 * @file free_list.h
 * @author Justin Tonkinson
 * @date 2024/06/27
 * @brief Free list function prototypes.
 */

#ifndef _DATA_FREE_LIST_H_
#define _DATA_FREE_LIST_H_

#include "data/linked_list.h"

#include <stddef.h>

/** @brief Internal headers for the unused blocks within a free list. */
typedef struct fnode_s {
    /** @brief The byte size of the current block. */
    size_t size;

    /**
     * @brief Linked list node that points to the next item in the free list. */
    LNODE(struct fnode_s);

    /** @brief The data section of the node. */
    void *data;
} fnode_t;

/** @brief Free list data structure. */
typedef struct {
    /** @brief The total byte size of the allocated block. */
    size_t size;

    /** @brief Linked list node that points to the free blocks of memory. */
    LLIST(fnode_t);

    /** @brief The data section of the free list. */
    void *data;
} flist_t;

/**
 * @brief Allocates a block of memory for a free list.
 * @param self a pointer to a free list
 * @param size number of bytes to allocate
 */
void flist_init(flist_t *, size_t size);

/**
 * @brief Releases the memeory allocated for a free list.
 * @param self a pointer to a free list
 */
void flist_destroy(flist_t *);

/**
 * @brief Returns a block of memory that can hold @a count number of items.
 * If there is not a block of memory available, the program will log the error
 * and terminate the program.
 * @param self a pointer to a free list
 * @param n the number of bytes being requested
 * @returns a pointer to a block of memory
 */
void *flist_request(flist_t *, size_t n);

/**
 * @brief Releases a block of memory from a free list.
 * The block of memory must be a pointer that was requested from the free list.
 * @see flist_request
 * @param self a pointer to a free list
 * @param data a pointer to the block of memory being freed
 */
void flist_release(flist_t *, void *data);

#endif // ifndef _DATA_FREE_LIST_H_

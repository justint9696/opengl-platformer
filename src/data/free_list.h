#ifndef _DATA_FREE_LIST_H_
#define _DATA_FREE_LIST_H_

#include "data/linked_list.h"

#include <stddef.h>

/* For the internal headers of unused blocks within a free list. */
typedef struct fnode_s {
    size_t capacity;
    LNODE(struct fnode_s);
    void *userdata;
} fnode_t;

typedef struct {
    size_t capacity, size;
    LLIST(fnode_t);
    void *data;
} flist_t;

/* Allocates a block of memory of (`size` * `capacity`) bytes. */
void flist_init(flist_t *, size_t size, size_t capacity);

/* Releases the memeory allocated for a free list. */
void flist_destroy(flist_t *);

/* Returns a block of memory that can hold `count` number of items. */
void *flist_request(flist_t *, size_t count);

/* Releases a block of memory from a free list; see `flist_request`. */
void flist_release(flist_t *, void *data);

#endif // ifndef _DATA_FREE_LIST_H_

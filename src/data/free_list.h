#ifndef _DATA_FREE_LIST_H_
#define _DATA_FREE_LIST_H_

#include "data/linked_list.h"

#include <stddef.h>

// for the internal headers of unused blocks within a free list
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

void flist_init(flist_t *, size_t size, size_t capacity);
void flist_destroy(flist_t *);

void *flist_request(flist_t *, size_t count);
void flist_release(flist_t *, void *data);

#endif // _DATA_FREE_LIST_H_

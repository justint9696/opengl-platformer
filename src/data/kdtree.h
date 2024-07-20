#ifndef _DATA_KDTREE_H_
#define _DATA_KDTREE_H_

#include <cglm/types-struct.h>
#include <stddef.h>

// implementation based on https://en.wikipedia.org/wiki/K-d_tree

// sorting function for kdtree data
typedef void (*sort_fn_t)(void *, size_t, short);

typedef struct kdnode_s {
    // reference to an object
    void *data;

    // reference to data->pos
    float *pos;

    // left and right children
    struct kdnode_s *left, *right;
} kdnode_t;

typedef struct {
    kdnode_t *root;
} kdtree_t;

void kdtree_init(kdtree_t *);
void kdtree_destroy(kdtree_t *);

void kdtree_insert(kdtree_t *, float *pos, void *data);
void *kdtree_nearest(kdtree_t *, float *);

void kdtree_from(kdtree_t *, void *arr, size_t len, int offset, sort_fn_t);

#endif // _DATA_KDTREE_H_

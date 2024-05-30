#ifndef _DATA_KDTREE_H_
#define _DATA_KDTREE_H_

#include <cglm/types-struct.h>
#include <stddef.h>

typedef void (*sort_fn_t)(void *, size_t, short);

typedef struct kdnode_s {
    vec2s pos;
    void *data;
    struct kdnode_s *left, *right;
} kdnode_t;

typedef struct {
    kdnode_t *root;
} kdtree_t;

void kdtree_init(kdtree_t *);
void kdtree_destroy(kdtree_t *);

void kdtree_insert(kdtree_t *, vec2s pos, void *data);
void kdtree_from(kdtree_t *, void *arr, size_t len, sort_fn_t);
void *kdtree_nearest(kdtree_t *, vec2s pos);

#endif

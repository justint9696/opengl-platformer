#ifndef _DATA_KDTREE_H_
#define _DATA_KDTREE_H_

#include <cglm/types-struct.h>
#include <stddef.h>

typedef struct tnode_t {
    vec2s pos;
    void *data;
    struct tnode_t *left, *right;
} tnode_t;

typedef struct {
    tnode_t *root;
} kdtree_t;

void kdtree_init(kdtree_t *);
void kdtree_destroy(kdtree_t *);

void kdtree_insert(kdtree_t *, vec2s pos, void *data);
void kdtree_from(kdtree_t *, void *arr, size_t len);
void *kdtree_nearest(kdtree_t *, vec2s pos);

#endif

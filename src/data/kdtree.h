#ifndef _DATA_KDTREE_H_
#define _DATA_KDTREE_H_

#include <cglm/types-struct.h>
#include <stddef.h>

/* Implementation based on https://en.wikipedia.org/wiki/K-d_tree */

/* Sorting function for kdtree data. */
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

/* Initializes a kdtree. */
void kdtree_init(kdtree_t *);

/* Releases the memory allocated for a kdtree. */
void kdtree_destroy(kdtree_t *);

/* Inserts an item at the appropriate leaf of a kdtree. */
void kdtree_insert(kdtree_t *, float *pos, void *data);

/* Returns the item of the node closest to the provided position. */
void *kdtree_nearest(kdtree_t *, float *);

/*
 * Creates a kdtree from the provided array.
 * The `offset` is the memory location of the `position` field within the
 * structures contained within the array. This be calculated with `offsetof()`.
 * Example:
 * ```
 * struct foo {
 *      int id;
 *      float pos[2];
 *  };
 *  int offset = offsetof(struct foo, pos);
 * ```
 */
void kdtree_from(kdtree_t *, void *arr, size_t len, int offset, sort_fn_t);

#endif // ifndef _DATA_KDTREE_H_

/**
 * @file kdtree.h
 * @author Justin Tonkinson
 * @date 2024/05/24
 * @brief KDTree function prototypes.
 * Implementation based on https://en.wikipedia.org/wiki/K-d_tree
 */

#ifndef _DATA_KDTREE_H_
#define _DATA_KDTREE_H_

#include <cglm/types-struct.h>
#include <stddef.h>

/** @brief Sorting function for kdtree data. */
typedef void (*sort_fn_t)(void *, size_t, int);

typedef struct kdnode_s {
    /** @brief A pointer to an object's data. */
    void *data;

    /** @brief A pointer to the position of an object. */
    float *pos;

    /** @brief The left and right children of the current node. */
    struct kdnode_s *left, *right;
} kdnode_t;

typedef struct {
    /** @brief The root of the kdtree. */
    kdnode_t *root;
} kdtree_t;

/**
 * @brief Initializes a kdtree.
 * @param self a pointer to a kdtree
 */
void kdtree_init(kdtree_t *);

/**
 * @brief Released the memory allocated for a kdtree.
 * @param self a pointer to a kdtree
 */
void kdtree_destroy(kdtree_t *);

/**
 * @brief Inserts an item into a kdtree.
 * The item will be inserted at the leaf that corresponds to the given position.
 * @param self a pointer to a kdtree
 * @param pos the world position of an object
 * @param data the object being inserted
 */
void kdtree_insert(kdtree_t *, float *pos, void *data);

/**
 * @brief Returns the data of the node in a kdtree that is closest to the given
 * position.
 * @param self a pointer to a kdtree
 * @param pos the world position of an object
 */
void *kdtree_nearest(kdtree_t *, float *);

/**
 * @brief Creates a kdtree from objects in a dynamic array.
 *
 * The objects are inserted into the kdtree by the depth of the current node
 * and the dimension of the position vector based on that depth. The sorting
 * function should sort the remaining objects by their x coordinate at depth 1,
 * by their y coordinate at depth 2, by their x coordinate at depth 3, and so
 * on.
 *
 * The `offset` is the memory location of the position field for the objects
 * contained within the array. This be calculated with `offsetof()`.
 * Example:
 * @code
 * struct foo { int id; float pos[2]; };
 * int offset = offsetof(struct foo, pos);
 * @endcode
 *
 * @param self a pointer to a kdtree
 * @param arr a pointer to the data section of a dynamic array
 * @param len the number of items in the array
 * @param offset the memory offset of the position field for the objects in the
 * array
 * @param sort the sorting function for the objects in the array
 */
void kdtree_from(kdtree_t *, void *arr, size_t len, int offset, sort_fn_t);

#endif // ifndef _DATA_KDTREE_H_

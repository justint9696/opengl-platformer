/**
 * @file kdtree.c
 * @author Justin Tonkinson
 * @date 2024/05/24
 * @brief KDTree implementation functions.
 */

#include "data/kdtree.h"

#include "data/array.h"

#include <assert.h>
#include <cglm/struct.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void kdtree_init(kdtree_t *self) {
    memset(self, 0, sizeof(kdtree_t));
}

static void destroy(kdnode_t *root) {
    if (!root)
        return;

    destroy(root->left);
    destroy(root->right);

    free(root);
}

void kdtree_destroy(kdtree_t *self) {
    destroy(self->root);
}

static kdnode_t *node_init(float *pos, void *data) {
    kdnode_t *self = calloc(1, sizeof(kdnode_t));
    assert(self);

    self->pos = pos;
    self->data = data;

    return self;
}

static void insert(kdnode_t **rootptr, float *pos, void *data, int depth) {
    kdnode_t *root = *rootptr;
    if (!root) {
        *rootptr = node_init(pos, data);
        return;
    }

    int axis = depth % 2;
    float a = pos[axis], b = root->pos[axis];
    insert(((a <= b) ? &root->left : &root->right), pos, data, depth + 1);
}

void kdtree_insert(kdtree_t *self, float *pos, void *data) {
    insert(&self->root, pos, data, 0);
}

static inline bool is_leaf(const kdnode_t *root) {
    return (!root->left && !root->right);
}

static void nearest_neighbor(kdnode_t *root, float *pos, float *distance,
                             kdnode_t *node, int depth) {
    // recursive base case
    if (!root)
        return;

    // if root node is a leaf and the distance < best distance
    float current = glm_vec2_distance(pos, root->pos);
    if (is_leaf(root) && current < *distance) {
        *distance = current;
        memcpy(node, root, sizeof(kdnode_t));
    }

    // traverse tree as normal
    int axis = depth % 2;
    float a = pos[axis], b = root->pos[axis];
    if (a <= b) {
        nearest_neighbor(root->left, pos, distance, node, depth + 1);

        // check if there could be a closer on the right side of split plane
        if (fabsf(a - b) < *distance) {
            nearest_neighbor(root->right, pos, distance, node, depth + 1);
        }
    } else {
        nearest_neighbor(root->right, pos, distance, node, depth + 1);

        // check if there could be a closer on the left side of split plane
        if (fabsf(a - b) < *distance) {
            nearest_neighbor(root->left, pos, distance, node, depth + 1);
        }
    }

    if (current < *distance) {
        *distance = current;
        memcpy(node, root, sizeof(kdnode_t));
    }
}

void *kdtree_nearest(kdtree_t *self, float *pos) {
    float distance = INT_MAX;
    kdnode_t node = { 0 };
    nearest_neighbor(self->root, pos, &distance, &node, 0);
    return node.data;
}

static void kdtree(kdnode_t **rootptr, void *arr[], size_t len, int depth,
                   int offset, sort_fn_t sort) {
    if (len < 1)
        return;

    // sort items in array along current axis
    int axis = depth % 2;
    sort(arr, len, axis);

    // get the position of the item at the middle index
    int mid = floorf(len / 2.f);
    void *data = arr[mid];
    float *pos = (float *)(data + offset);

    // insert position and data pointer
    *rootptr = node_init(pos, data);

    // ensure array can be sliced
    if (mid == 0)
        return;

    // slice array from 0 to mid
    size_t n = mid;
    kdtree(&((*rootptr)->left), arr, n, depth + 1, offset, sort);

    // ensure there are items on the right side of mid
    if (len - mid <= 1)
        return;

    // slice array from (mid + 1) to len
    n = (len - mid - 1);
    kdtree(&((*rootptr)->right), &arr[mid + 1], n, depth + 1, offset, sort);
}

void kdtree_from(kdtree_t *self, void *arr, size_t len, int offset,
                 sort_fn_t sort) {
    // initialize kdtree
    kdtree_init(self);

    // create a stack allocated array of pointers to items in arr
    void *points[len];
    for (size_t i = 0; i < len; i++) {
        points[i] = array_at(arr, i);
    }

    // build kdtree from array of pointers
    kdtree(&self->root, points, len, 0, offset, sort);
}

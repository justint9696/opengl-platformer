#include "data/kdtree.h"

#include "data/array.h"
#include "tile/tile.h"
#include "util/aabb.h"

#include <assert.h>
#include <cglm/struct.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void kdtree_init(kdtree_t *self) {
    memset(self, 0, sizeof(kdtree_t));
    self->root = NULL;
}

static void destroy(tnode_t *root) {
    if (!root)
        return;

    destroy(root->left);
    destroy(root->right);

    free(root);
}

void kdtree_destroy(kdtree_t *self) {
    destroy(self->root);
}

static tnode_t *node_init(vec2s pos, void *data) {
    tnode_t *self = calloc(1, sizeof(tnode_t));
    assert(self);
    self->pos = pos;
    self->data = data;
    return self;
}

static void insert(tnode_t **rootptr, vec2s pos, void *data, short depth) {
    tnode_t *root = *rootptr;
    if (!root) {
        *rootptr = node_init(pos, data);
        return;
    }

    short axis = depth % 2;
    float a = pos.raw[axis], b = root->pos.raw[axis];
    insert((a <= b ? &root->left : &root->right), pos, data, depth + 1);
}

void kdtree_insert(kdtree_t *self, vec2s pos, void *data) {
    insert(&self->root, pos, data, 0);
}

static int tile_compare_x(const void *a, const void *b) {
    const tile_t *x = *(const tile_t **)a;
    const tile_t *y = *(const tile_t **)b;

    if (x->body.pos.x < y->body.pos.x)
        return -1;
    else if (x->body.pos.x > y->body.pos.x)
        return 1;
    return 0;
}

static int tile_compare_y(const void *a, const void *b) {
    const tile_t *x = *(const tile_t **)a;
    const tile_t *y = *(const tile_t **)b;

    if (x->body.pos.y < y->body.pos.y)
        return -1;
    else if (x->body.pos.y > y->body.pos.y)
        return 1;
    return 0;
}

static void kdtree(tnode_t **rootptr, void *arr[], size_t len, short depth) {
    if (len < 1)
        return;

    switch (depth % 2) {
        case 0:
            qsort(arr, len, sizeof(void *), tile_compare_x);
            break;
        case 1:
            qsort(arr, len, sizeof(void *), tile_compare_y);
            break;
        default:
            break;
    }

    short mid = floorf(len / 2.f);
    tile_t *tile = arr[mid];
    const box_t *box = &tile->body.box;
    vec2s center = box_center(box);
    *rootptr = node_init(center, tile);

    // ensure array can be sliced
    if (mid == 0)
        return;

    // slice array from 0 to mid
    len = mid;
    void *left[len];
    memcpy(left, arr, sizeof(void *) * len);
    kdtree(&((*rootptr)->left), left, len, depth + 1);

    // slice array from mid to len
    len = (len - mid + 1);
    void *right[len];
    memcpy(right, &arr[mid + 1], sizeof(void *) * len);
    kdtree(&((*rootptr)->right), right, len, depth + 1);
}

void kdtree_from(kdtree_t *self, void *arr, size_t len) {
    // initialize kdtee
    kdtree_init(self);

    // create a stack allocated array of pointers to items in arr
    void *points[len];
    for (size_t i = 0; i < len; i++) {
        points[i] = array_get(arr, i);
    }

    // build kdtree with array of pointers
    kdtree(&self->root, points, len, 0);
}

static bool is_leaf(const tnode_t *root) {
    return (!root->left && !root->right);
}

static void *nearest_neighbor(tnode_t *root, vec2s pos, float *distance,
                              short depth) {
    assert(root);
    if (is_leaf(root)) {
        float current = glms_vec2_distance2(pos, root->pos);
        if (current < *distance) {
            *distance = current;
            return root;
        }
        return NULL;
    }

    short axis = depth % 2;
    float a = pos.raw[axis], b = root->pos.raw[axis];

    tnode_t *node = NULL;
    if (root->left && (a <= b || !root->right)) {
        // if item->pos <= root->pos or there is not a right child
        node = nearest_neighbor(root->left, pos, distance, depth + 1);

        // check to see if right child could have a closer node
        if (root->right
            && glms_vec2_distance2(pos, root->right->pos) < *distance) {
            node = nearest_neighbor(root->right, pos, distance, depth + 1);
        }
    } else if (root->right && (a > b || !root->left)) {
        // if item->pos > root->pos or there is not a left child
        node = nearest_neighbor(root->right, pos, distance, depth + 1);

        // check to see if left child could have a closer node
        if (root->left
            && glms_vec2_distance2(pos, root->left->pos) < *distance) {
            node = nearest_neighbor(root->left, pos, distance, depth + 1);
        }
    }

    float current = glms_vec2_distance2(pos, root->pos);
    if (!node || current < *distance) {
        *distance = current;
        return root;
    }

    return node;
}

void *kdtree_nearest(kdtree_t *self, vec2s pos) {
    float distance = INT_MAX;
    tnode_t *node = nearest_neighbor(self->root, pos, &distance, 0);
    return node->data;
}

#include "data/free_list.h"

#include "util/log.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define list_header(_d)\
    ((fnode_t *)(((void *)(_d)) - offsetof(fnode_t, userdata)))

void flist_init(flist_t *self, size_t size, size_t capacity) {
    memset(self, 0, sizeof(flist_t));

    self->data = calloc(1, sizeof(fnode_t) + (size * capacity));
    assert(self->data);

    self->size = size;
    self->capacity = capacity;

    self->head = self->data;
    self->head->capacity = capacity;
}

void flist_destroy(flist_t *self) {
    assert(self->data);
    free(self->data);
}

void *flist_request(flist_t *self, size_t count) {
    for (fnode_t *tmp = self->head, *prev = NULL; tmp;
         prev = tmp, tmp = tmp->next) {
        // if the current node has enough requested space
        if (tmp->capacity >= count) {
            // initialize user data
            memset(&tmp->userdata, 0, (count * self->size));

            // calculate the memory remaining from the requested segment
            size_t bytes = ((tmp->capacity - count) * self->size);

            // check if there is enough memory for another next header
            if (bytes <= offsetof(fnode_t, userdata)) {
                log_warn("No memory available after request.\n");
                llist_remove(self, prev, tmp);
                return &tmp->userdata;
            }

            // calculate the position of the next header
            fnode_t *node = (((void *)tmp) + (count * self->size)
                             + offsetof(fnode_t, userdata));

            // update the next header's parameters
            node->capacity
                = ((bytes - offsetof(fnode_t, userdata)) / self->size);

            // insert next header into linked list, replacing the current header
            llist_replace(self, prev, tmp, node);

            // update current header's parameters
            tmp->capacity = count;
            tmp->next = NULL;

            // return the requested block of memory
            return &tmp->userdata;
        }
    }

    log_and_fail("Requested memory was not available.\n");
}

static void consolidate_segs(flist_t *self, fnode_t *prev, fnode_t *src,
                             fnode_t *dst) {
    size_t bytes = ((dst->capacity + src->capacity) * self->size);
    dst->capacity = ((bytes + offsetof(fnode_t, userdata)) / self->size);
    llist_replace(self, prev, src, dst);
}

static inline bool adjacent_segs(const flist_t *self, const fnode_t *a,
                                 const fnode_t *b) {
    return (((void *)&a->userdata + (self->size * a->capacity) == b)
            || ((void *)&b->userdata + (self->size * b->capacity) == a));
}

static void defrag_segs(flist_t *self, fnode_t *prev, fnode_t *tmp,
                        fnode_t *node) {
    if (tmp && adjacent_segs(self, tmp, node)) {
        // combine adjacent segments and replace tmp with node
        consolidate_segs(self, prev, tmp, node);
    }

    if (prev && adjacent_segs(self, prev, node)) {
        // combine adjacent segments and replace node with prev
        consolidate_segs(self, NULL, node, prev);
    }
}

void flist_release(flist_t *self, void *data) {
    // calculate data header
    fnode_t *node = list_header(data);

    // if there is not a head, insert node into linked list as head
    if (!self->head) {
        self->head = node;
        return;
    }

    // traverse linked list to find where the node belongs in ascending memory
    fnode_t *tmp, *prev;
    for (tmp = self->head, prev = NULL; tmp; prev = tmp, tmp = tmp->next) {
        if ((tmp == self->head && node < self->head)
            || (prev && (node > prev && node < tmp))) {
            break;
        }
    }

    // insert node into linked list and defragment segments if needed
    llist_insert(self, prev, tmp, node);
    defrag_segs(self, prev, tmp, node);
}

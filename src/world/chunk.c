#include "world/chunk.h"

#include "data/array.h"
#include "data/free_list.h"

#include <assert.h>
#include <math.h>
#include <string.h>

static flist_t entities;

void chunk_init(chunk_t *self) {
    memset(self, 0, sizeof(chunk_t));

    size_t capacity = ceilf(CHUNK_MAX * 9.25f);
    flist_init(&entities, sizeof(entity_t), capacity);

    void *ptr = NULL;
    for (short i = 0; i < 9; i++) {
        page_t *page = &self->pages[i];
        assert(page);

        ptr = flist_request(&entities, CHUNK_MAX);
        page->entities = array_init(ptr, sizeof(entity_t), CHUNK_MAX);

        page->index = i;
    }
}

void chunk_destroy(chunk_t *self) {
    flist_destroy(&entities);
}

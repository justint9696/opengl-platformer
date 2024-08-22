#include "world/chunk.h"

#include "data/array.h"
#include "data/free_list.h"
#include "graphics/drawing.h"

#include <assert.h>
#include <cglm/struct.h>
#include <string.h>

static flist_t entities;

void chunk_init(chunk_t *self) {
    memset(self, 0, sizeof(chunk_t));

    size_t capacity = (CHUNK_MAX * sizeof(array_t) * 9);
    flist_init(&entities, sizeof(entity_t), capacity);

    for (int i = 0; i < 9; i++) {
        page_t *page = &self->pages[i];
        assert(page);

        page->entities = chunk_request_page(self, page);
        page->index = i;
    }
}

void chunk_destroy(chunk_t *self) {
    flist_destroy(&entities);
}

void chunk_render(chunk_t *self) {
    for (int i = 0; i < 9; i++) {
        page_t *page = &self->pages[i];
        assert(page);

        draw_quad_line(page->pos, page->dim, COLOR_RED);
    }
}

page_t *chunk_page_from_pos(chunk_t *self, vec2s pos) {
    vec2s coord = glms_vec2_divs(glms_vec2_sub(pos, self->pos), CHUNK_SIZE);
    uint32_t index = ((floorf(coord.y) * CHUNK_WIDTH) + floorf(coord.x));
    assert(index < 9);
    return &self->pages[index];
}

int chunk_index_from_pos(chunk_t *self, vec2s pos) {
    // calculate the offset from the center chunk
    vec2s coord = glms_vec2_subs(
        glms_vec2_divs(glms_vec2_sub(pos, self->pos), CHUNK_SIZE), 1.f);

    // calculate the offset of the center chunk with respects to the chunk
    // dimensions
    vec2s idx = (vec2s) {
        .x = self->index % self->dim.x,
        .y = floorf(1.f * self->index / self->dim.x),
    };

    idx = glms_vec2_add(idx, coord);

    // this allows (-1, -1) to be the top left page
    // instead of the bottom left page.
    idx.y = self->dim.y - idx.y - 1;

    if ((idx.x < 0 || idx.x >= self->dim.x)
        || (idx.y < 0 || idx.y > self->dim.y))
        return -1;

    int32_t index = ((floorf(idx.y) * self->dim.x) + floorf(idx.x));
    return index;
}

void *chunk_request_page(chunk_t *self, page_t *page) {
    void *ptr = flist_request(&entities, CHUNK_MAX);
    return array_init(ptr, sizeof(entity_t), CHUNK_MAX);
}

void chunk_release_page(chunk_t *self, page_t *page) {
    flist_release(&entities, array_header(page->entities));
}

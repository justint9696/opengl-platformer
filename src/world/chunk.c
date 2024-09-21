/**
 * @brief chunk.c
 * @author Justin Tonkinson
 * @date 2024/07/04
 * @brief Chunk implementation functions.
 */

#include "world/chunk.h"

#include "data/array.h"
#include "data/free_list.h"
#include "graphics/drawing.h"
#include "util/log.h"

#include <assert.h>
#include <cglm/struct.h>
#include <string.h>

/** @brief Internal free list that manages the memory of the current pages. */
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
    vec2s coord = glms_vec2_divs(glms_vec2_sub(pos, self->pos), CHUNK_SIZE);
    glms_vec2_print(pos, stdout);
    glms_vec2_print(coord, stdout);
    return fabsf(
        floorf(coord.x)
        + ((self->box.dim.y - floorf(fabsf(coord.y)) - 1) * self->box.dim.x));
}

void *chunk_request_page(chunk_t *self, page_t *page) {
    void *ptr = flist_request(&entities, CHUNK_MAX);
    return array_init(ptr, sizeof(entity_t), CHUNK_MAX);
}

void chunk_release_page(chunk_t *self, page_t *page) {
    flist_release(&entities, array_header(page->entities));
}

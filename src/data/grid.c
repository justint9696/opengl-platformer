/**
 * @file grid.c
 * @author Justin Tonkinson
 * @date 2024/05/24
 * @brief Dynamic array implementation functions.
 */

#include "data/grid.h"

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#define offset(_x, _y, _w) ((_x) + ((_y) * (_w)))

void grid_init(grid_t *self, int size, int width, int height) {
    memset(self, 0, sizeof(grid_t));
    assert(((width / size) * (height / size) < BUCKET_MAX));
    self->size = size;
    self->dim = (ivec2s) {
        .x = ceilf(1.f * width / size),
        .y = ceilf(1.f * height / size),
    };
    for (int y = 0; y < self->dim.y; y++) {
        for (int x = 0; x < self->dim.x; x++) {
            assert(offset(x, y, self->dim.x) < BUCKET_MAX);
            cell_t *cell = &self->bucket[offset(x, y, self->dim.x)];
            cell->pos = (ivec2s) { x, y };
        }
    }
}

void grid_insert(grid_t *self, vec2s pos, void *data) {
    cell_t *cell = grid_cell_from_pos(self, pos);
    cell_insert(cell, data);
}

void grid_remove(grid_t *self, vec2s pos, void *data) {
    cell_t *cell = grid_cell_from_pos(self, pos);
    cell_remove(cell, data);
}

cell_t *grid_cell_from_pos(grid_t *self, vec2s pos) {
    if (pos.x < 0 || pos.y < 0)
        return NULL;

    uint32_t x = floorf(pos.x / self->size);
    uint32_t y = floorf(pos.y / self->size);
    return &self->bucket[offset(x, y, self->dim.x)];
}

cell_t *grid_cell_from_index(grid_t *self, ivec2s index) {
    return &self->bucket[offset(index.x, index.y, self->dim.x)];
}

void cell_insert(cell_t *cell, void *data) {
    assert(cell->len < CELL_MAX);
    cell->arr[cell->len++] = data;
}

void cell_remove(cell_t *cell, void *data) {
    size_t len = cell->len;

    for (size_t i = 0; i < len; i++) {
        if (cell->arr[i] == data) {
            memcpy(&cell->arr[i], &cell->arr[i + 1],
                   sizeof(void *) * --cell->len);
            break;
        }
    }
}

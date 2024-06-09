#include "data/grid.h"

#include "data/array.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define offset(_x, _y, _w) ((_x) + ((_y) * (_w)))

// 640x480 produces a 13x10 grid with cells of size 50
void grid_init(grid_t *self, ivec2s cell_size, ivec2s res) {
    memset(self, 0, sizeof(grid_t));
    self->dim = (ivec2s) {
        .x = ceilf(1.f * res.x / cell_size.x),
        .y = ceilf(1.f * res.y / cell_size.y),
    };
    self->size = cell_size;

    self->cells = calloc(self->dim.x * self->dim.y, sizeof(cell_t));
    assert(self->cells);

    for (int y = 0; y < self->dim.y; y++) {
        for (int x = 0; x < self->dim.x; x++) {
            cell_t *cell = &self->cells[offset(x, y, self->dim.x)];
            cell->pos = (ivec2s) { x, y };
        }
    }
}

void grid_destroy(grid_t *self) {
    for (int y = 0; y < self->dim.y; y++) {
        for (int x = 0; x < self->dim.x; x++) {
            cell_t *cell = &self->cells[offset(x, y, self->dim.x)];
            if (!cell->items)
                continue;

            array_free(cell->items);
        }
    }

    assert(self->cells);
    free(self->cells);
}

void grid_add(grid_t *self, void *data, vec2s pos) {
    cell_t *cell = grid_cell_pos(self, pos);
    if (!cell->items) {
        cell->items = array_alloc(sizeof(void *), 16);
    }

    assert(cell->items);
    array_push(&cell->items, data);
}

void grid_remove(grid_t *self, void *data, vec2s pos) {
    cell_t *cell = grid_cell_pos(self, pos);
    if (!cell->items) return;

    array_remove(cell->items, data);
}

void grid_update(grid_t *self, void *data, vec2s prev_pos, vec2s pos) {
    cell_t *prev = grid_cell_pos(self, prev_pos);
    cell_t *curr = grid_cell_pos(self, pos);

    if (prev == curr) return;

    grid_remove(self, data, prev_pos);
    grid_add(self, data, pos);
}

cell_t *grid_cell_pos(grid_t *self, vec2s pos) {
    int x = floorf(pos.x / self->size.x);
    int y = floorf(pos.y / self->size.y);
    return &self->cells[offset(x, y, self->dim.x)];
}

cell_t *grid_cell_index(grid_t *self, ivec2s index) {
    return &self->cells[offset(index.x, index.y, self->dim.x)];
}

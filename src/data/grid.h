#ifndef _DATA_GRID_H_
#define _DATA_GRID_H_

#include <cglm/types-struct.h>

typedef struct {
    ivec2s pos;
    void *items;
} cell_t;

typedef struct {
    ivec2s dim;
    ivec2s size;
    cell_t *cells;
} grid_t;

void grid_init(grid_t *, ivec2s cell_size, ivec2s res);
void grid_destroy(grid_t *);

void grid_add(grid_t *, void *data, vec2s pos);
void grid_remove(grid_t *, void *data, vec2s pos);

void grid_update(grid_t *, void *data, vec2s prev_pos, vec2s pos);

cell_t *grid_cell_pos(grid_t *, vec2s pos);
cell_t *grid_cell_index(grid_t *, ivec2s index);

#endif

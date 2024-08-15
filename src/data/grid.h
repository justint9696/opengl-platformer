#ifndef _DATA_GRID_H_
#define _DATA_GRID_H_

#include <cglm/types-struct.h>
#include <stddef.h>

#define BUCKET_MAX 128
#define CELL_MAX   32

typedef struct cell_s {
    ivec2s pos;
    size_t len;
    void *arr[CELL_MAX];
} cell_t;

typedef struct {
    int size;
    ivec2s dim;
    cell_t bucket[BUCKET_MAX];
} grid_t;

/* Initializes a grid with the provided parameters. */
void grid_init(grid_t *, int size, int width, int height);

/* Inserts an item into the grid cell that corresponds to the provided position.
 */
void grid_insert(grid_t *, vec2s pos, void *data);

/* Removes an item from the grid cell that corresponds to the provided position.
 */
void grid_remove(grid_t *, vec2s pos, void *data);

/* Returns the cell that corresponds to the provided position. */
cell_t *grid_cell_from_pos(grid_t *, vec2s pos);

/* Returns the cell at the (x, y) coordinates of the provided index. */
cell_t *grid_cell_from_index(grid_t *, ivec2s index);

/* Inserts an item into the provided cell. */
void cell_insert(cell_t *, void *data);

/* Removes an item from the provided cell. */
void cell_remove(cell_t *, void *data);

#endif // ifndef _DATA_GRID_H_

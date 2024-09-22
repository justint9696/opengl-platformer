/**
 * @file grid.h
 * @author Justin Tonkinson
 * @date 2024/05/24
 * @brief Uniform grid function prototypes.
 */

#ifndef _DATA_GRID_H_
#define _DATA_GRID_H_

#include <cglm/types-struct.h>
#include <stddef.h>

#define BUCKET_MAX 1024
#define CELL_MAX   32

/**
 * @brief The cells of the uniform grid that hold objects in a certain region
 * of the screen.
 */
typedef struct cell_s {
    /** @brief The (x, y) coordinates of the cell within the uniform grid. */
    ivec2s pos;

    /** @brief The number of items in the cell. */
    size_t len;

    /** @brief An array of pointers to the items in the cell. */
    void *arr[CELL_MAX];
} cell_t;

/** @brief Uniform grid data structure. */
typedef struct grid_s {
    /** @brief The size of each cell in the grid. */
    int size;

    /** @brief The length and width of the grid based on the cell size. */
    ivec2s dim;

    /** @brief The grid bucket that holds all the cells. */
    cell_t bucket[BUCKET_MAX];
} grid_t;

/**
 * @brief Initializes a grid with the provided parameters.
 * @param self a pointer to a grid
 * @param size the size of the cells within the grid
 * @param width the size of the screen
 * @param height the height of the screen
 */
void grid_init(grid_t *, int size, int width, int height);

/**
 * @brief Inserts an item into the grid cell that corresponds to the given
 * position.
 * @param self a pointer to a grid
 * @param pos the screen position of an item
 * @param data a pointer to the item being inserted
 */
void grid_insert(grid_t *, vec2s pos, void *data);

/**
 * @brief Removes an item from the grid cell that corresponds to the given
 * position.
 * @param self a pointer to a grid
 * @param pos the screen position of an item
 * @param data a pointer to the item being inserted
 */
void grid_remove(grid_t *, vec2s pos, void *data);

/**
 * @brief Returns the cell of a grid that corresponds to the given position.
 * @param self a pointer to a grid
 * @param pos the screen position of an item
 * @returns a cell corresponding to the given screen position
 */
cell_t *grid_cell_from_pos(grid_t *, vec2s pos);

/**
 * @brief Returns the cell at the (x, y) coordinates of the provided index.
 * @param self a pointer to a grid
 * @param index the indices (x and y coordinates) of a grid
 * @returns a cell corresponding to the given indices
 */
cell_t *grid_cell_from_index(grid_t *, ivec2s index);

/**
 * @brief Inserts an item into the provided cell.
 * @param cell a pointer to the cell of a grid
 * @param data a pointer to the item being inserted
 */
void cell_insert(cell_t *, void *data);

/**
 * @brief Removes an item from the provided cell.
 * @param cell a pointer to the cell of a grid
 * @param data a pointer to the item being removed
 */
void cell_remove(cell_t *, void *data);

#endif // ifndef _DATA_GRID_H_

/**
 * @file level.h
 * @author Justin Tonkinson
 * @date 2024/06/10
 * @brief Level data structures and function prototypes.
 * @details The structure of level data files is as follows:
 * @code
 * int      origin;
 * ivec2s   dchunk; // (width, height)
 * ldata_t  player;
 * for (0 .. nchunk) {
 *     size_t nent;
 *     ldata_t entities[nent];
 * }
 * @endcode
 */

#ifndef _LEVEL_LEVEL_H_
#define _LEVEL_LEVEL_H_

#include "world/world.h"

#include <stdio.h>

/** @brief Level data representation of entities. */
typedef struct ldata_s {
    int type;
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };
} ldata_t;

/** @brief Level data structure. */
typedef struct level_s {
    /** @brief Dynamic array of page offets within the level file. */
    uint64_t *offsets;

    /** @brief Pointer to temporary file containing padded level data. */
    FILE *fp;
} level_t;

/**
 * @brief Loads data from a level file into the game world.
 *
 * Initializes the internal level state and creates a temporary file with
 * padding to support adding or removing additional entities in pages.
 *
 * @param world a pointer to the game world
 * @param fpath relative file path to a level file
 */
void level_import(world_t *, const char *fpath);

/**
 * @brief Exports data from the world into a file.
 * @param world a pointer to the game world
 * @param fpath relative path to export location
 */
void level_export(world_t *, const char *fpath);

/**
 * @brief Releases the data associated with the level.
 * @param world a pointer to the game world
 */
void level_shutdown(world_t *);

/**
 * @brief Changes the primary page and swaps others accordingly.
 *
 * The @a page parameter determines how the other pages should be shifted
 * around. For example, if the player moves to the top right page (page index
 * 2), then the other pages should be shifted down and left so that page 2 is
 * now the center page.
 *
 * @param world a pointer to the game world
 * @param page a pointer to a page within the chunk
 */
void level_swap_pages(world_t *, page_t *);

#endif // ifndef _LEVEL_LEVEL_H_

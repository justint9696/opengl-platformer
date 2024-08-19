#ifndef _LEVEL_LEVEL_H_
#define _LEVEL_LEVEL_H_

#include "world/world.h"

#include <stdio.h>

/*
 * level.dat file format
 * int              origin;
 * ivec2s           dchunk; // (width, height)
 * ldata_t          player;
 *
 * for (i = 0; i < nchunk; i++) {
 *     size_t       nent;
 *     ldata_t      entities[nent];
 * }
 */

/* Level data representation of entities. */
typedef struct {
    int type;
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };
} ldata_t;

typedef struct {
    // array of offets within level file
    uint32_t *offsets;

    // pointer to file containing level data
    FILE *fp;
} level_t;

/* Loads data from a level file into the world */
void level_import(world_t *, const char *fpath);

/* Exports data from the world into a file */
void level_export(world_t *, const char *fpath);

/* Releases the data associated with the level. */
void level_shutdown(world_t *);

/* Changes the primary page and swaps others accordingly. */
void level_swap_pages(world_t *, page_t *);

#endif // ifndef _LEVEL_LEVEL_H_

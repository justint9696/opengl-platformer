#ifndef _LEVEL_LEVEL_H_
#define _LEVEL_LEVEL_H_

#include "world/world.h"

#include <cglm/types-struct.h>
#include <stdint.h>
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

// corresponding to entities and tiles
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
    // position offset of the current level file
    int offset;

    // level file pointer
    FILE *fp;
} level_t;

void level_import(level_t *, world_t *, const char *fpath);
void level_export(const level_t *, const world_t *, const char *fpath);

void level_swap_chunks(level_t *, world_t *, uint32_t idx);

#endif // _LEVEL_LEVEL_H_

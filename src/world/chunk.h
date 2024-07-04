#ifndef _WORLD_CHUNK_H_
#define _WORLD_CHUNK_H_

#include "entity/entity.h"
#include "tile/tile.h"

#define CHUNK_MAX  256
#define CHUNK_SIZE 32

typedef struct {
    // index used to get surrounding pages
    int index;

    // array of entities within current page
    entity_t *entities;

    // array of tiles within current page
    tile_t *tiles;
} page_t;

typedef struct chunk_s {
    // center point of the level
    vec2s origin;

    // current pages loaded into memory
    page_t pages[9];
} chunk_t;

void chunk_init(chunk_t *);
void chunk_destroy(chunk_t *);

#endif // _WORLD_CHUNK_H_

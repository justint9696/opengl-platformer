#ifndef _WORLD_CHUNK_H_
#define _WORLD_CHUNK_H_

#include "entity/entity.h"

#include <stdint.h>

#define CHUNK_SIZE  384
#define CHUNK_MAX   64
#define CHUNK_WIDTH 3

typedef struct page_s {
    // index used to get surrounding pages
    int32_t index;

    // bounds of the current page
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    // array of entities within the current page
    entity_t *entities;
} page_t;

typedef struct {
    // index of the center chunk
    int32_t index;

    // size of the level
    ivec2s dim;

    // position in world space
    vec2s pos;

    // current pages loaded into memory
    page_t pages[9];
} chunk_t;

/* Initializes the world chunk. */
void chunk_init(chunk_t *);

/* Releases the memory allocated for the world chunk. */
void chunk_destroy(chunk_t *);

/* Renders the bounds of the world chunk. */
void chunk_render(chunk_t *);

/* Returns the page asscoiated with the given position. */
page_t *chunk_page_from_pos(chunk_t *, vec2s pos);

/* Returns the chunk index associated with the given position. */
int chunk_index_from_pos(chunk_t *, vec2s pos);

/* Returns a free block of memory that is initialized as an array. */
void *chunk_request_page(chunk_t *, page_t *);

/* Releases an occupied block of memory and marks it as free. */
void chunk_release_page(chunk_t *, page_t *);

#endif // ifndef _WORLD_CHUNK_H_

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

void chunk_init(chunk_t *);
void chunk_destroy(chunk_t *);
void chunk_render(chunk_t *);

page_t *chunk_page_from_pos(chunk_t *, vec2s pos);

void *chunk_request_page(chunk_t *, page_t *);
void chunk_release_page(chunk_t *, page_t *);

#endif // ifndef _WORLD_CHUNK_H_

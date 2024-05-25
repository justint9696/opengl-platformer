#ifndef _TILE_TILE_H_
#define _TILE_TILE_H_

#include "game/types.h"

#include <inttypes.h>

struct world_t;

typedef struct tile_t {
    uint32_t id;
    static_body_t body;

    void (*init)(struct tile_t *, struct world_t *);
    void (*destroy)(struct tile_t *, struct world_t *);
    void (*render)(struct tile_t *, struct world_t *);
} tile_t;

tile_t *tile_create(void *data, struct world_t *);
void tile_destroy(tile_t *, struct world_t *);
void tile_render(tile_t *, struct world_t *);

#endif

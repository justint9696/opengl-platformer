#ifndef _TILE_TILE_H_
#define _TILE_TILE_H_

#include "game/types.h"

#include <stdint.h>

struct world_s;

typedef enum {
    TT_PLATFORM,
} tiletype_e;

typedef struct tile_s {
    uint32_t id;
    tiletype_e type;
    staticbody_t body;

    void (*init)(struct tile_s *, struct world_s *);
    void (*destroy)(struct tile_s *, struct world_s *);
    void (*render)(struct tile_s *, struct world_s *);
} tile_t;

void tile_sort(void *arr, size_t len, short axis);

tile_t *tile_create(void *data, struct world_s *);
void tile_destroy(tile_t *, struct world_s *);
void tile_render(tile_t *, struct world_s *);

#endif // _TILE_TILE_H_

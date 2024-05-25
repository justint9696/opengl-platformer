#ifndef _ENTITY_ENTITY_H_
#define _ENTITY_ENTITY_H_

#include "game/types.h"

#include <inttypes.h>

struct world_t;

typedef enum {
    F_NONE              = 0,
    F_PLAYER_CONTROLLED = 1 << 0,
    F_AI_CONTROLLED     = 1 << 1,
    F_KINEMATIC         = 1 << 2,
    F_GRAVITY           = 1 << 3,
} flag_t;

typedef struct entity_t {
    uint32_t id;
    flag_t flags;
    rigid_body_t body;

    void (*init)(struct entity_t *, struct world_t *);
    void (*destroy)(struct entity_t *, struct world_t *);
    void (*render)(struct entity_t *, struct world_t *);
    void (*tick)(struct entity_t *, struct world_t *, float dt);
    void (*update)(struct entity_t *, struct world_t *, float dt);
} entity_t;

entity_t *entity_create(void *data, struct world_t *);
void entity_destroy(entity_t *, struct world_t *);
void entity_render(entity_t *, struct world_t *);
void entity_tick(entity_t *, struct world_t *, float dt);
void entity_update(entity_t *, struct world_t *, float dt);

#endif

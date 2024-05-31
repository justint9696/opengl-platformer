#ifndef _ENTITY_ENTITY_H_
#define _ENTITY_ENTITY_H_

#include "game/types.h"

#include <inttypes.h>

struct world_s;

typedef enum {
    F_NONE              = 0,
    F_PLAYER_CONTROLLED = 1,
    F_AI_CONTROLLED     = 2,
    F_KINEMATIC         = 4,
    F_GRAVITY           = 8,
} flag_t;

typedef struct entity_s {
    uint32_t id;
    uint32_t flags;
    rigid_body_t body;

    void (*init)(struct entity_s *, struct world_s *);
    void (*destroy)(struct entity_s *, struct world_s *);
    void (*render)(struct entity_s *, struct world_s *);
    void (*tick)(struct entity_s *, struct world_s *, float dt);
    void (*update)(struct entity_s *, struct world_s *, float dt);
} entity_t;

entity_t *entity_create(void *data, struct world_s *);
void entity_destroy(entity_t *, struct world_s *);
void entity_render(entity_t *, struct world_s *);
void entity_tick(entity_t *, struct world_s *, float dt);
void entity_update(entity_t *, struct world_s *, float dt);

#endif

#pragma once

#include "game/types.h"

#include <inttypes.h>

struct world_s;

typedef enum {
    FLAG_KINEMATIC = 1 << 0,
    FLAG_PHYSICS = 1 << 1,
    FLAG_PLAYER_INPUT = 1 << 2,
} flag_t;

typedef struct entity_s {
    uint32_t id;
    flag_t flags;
    rigid_body_s body;

    void (*init)(struct entity_s *, struct world_s *);
    void (*destroy)(struct entity_s *, struct world_s *);
    void (*render)(struct entity_s *, struct world_s *);

    void (*tick)(struct entity_s *, struct world_s *, float dt);
    void (*update)(struct entity_s *, struct world_s *, float dt);
} entity_s;

entity_s *entity_create(void *data, struct world_s *);
void entity_destroy(entity_s *, struct world_s *);
void entity_render(entity_s *, struct world_s *);

void entity_tick(entity_s *, struct world_s *, float dt);
void entity_update(entity_s *, struct world_s *, float dt);

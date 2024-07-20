#ifndef _ENTITY_ENTITY_H_
#define _ENTITY_ENTITY_H_

#include "game/types.h"

#include <stdint.h>

struct world_s;

typedef enum {
    F_NONE              = 0,
    F_PLAYER_CONTROLLED = 1 << 0,
    F_AI_CONTROLLED     = 1 << 1,
    F_KINEMATIC         = 1 << 2,
    F_GRAVITY           = 1 << 3,
    F_CAMERA_FOLLOW     = 1 << 4,
} flag_e;

typedef enum {
    ET_PLAYER,
    ET_ENEMY,
    ET_PLATFORM,
    ET_MAX,
} entity_e;

typedef struct entity_s {
    int32_t id;
    int32_t flags;
    int32_t buttons;
    entity_e type;
    body_t body;

#ifdef _DEBUG
    struct {
        vec2s *lines;
    } debug;
#endif // _DEBUG

    void (*init)(struct entity_s *, struct world_s *);
    void (*destroy)(struct entity_s *, struct world_s *);
    void (*render)(struct entity_s *, struct world_s *);
    void (*sync)(struct entity_s *, struct world_s *, float dt);
    void (*update)(struct entity_s *, struct world_s *, float dt);
} entity_t;

entity_t *entity_create(void *data, struct world_s *);
void entity_destroy(entity_t *, struct world_s *);
void entity_render(entity_t *, struct world_s *);
void entity_sync(entity_t *, struct world_s *, float dt);
void entity_update(entity_t *, struct world_s *, float dt);

#endif // _ENTITY_ENTITY_H_

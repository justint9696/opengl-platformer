#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include "util/aabb.h"

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <time.h>

struct entity_t;
struct tile_t;

typedef struct {
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    vec2s vel;
    bool solid;
    bool grounded;
    float jump_speed;
    float movement_speed;
    time_t grounded_tick;
    void (*collision_callback)(struct entity_t *, struct entity_t *);
} rigid_body_t;

typedef struct {
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    bool solid;
    void (*collision_callback)(struct tile_t *, struct entity_t *);
} static_body_t;

#endif

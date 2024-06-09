#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include "util/aabb.h"

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <time.h>

struct entity_s;
struct tile_s;

typedef struct {
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    vec2s vel;
    vec2s accel;
    vec2s direction;

    bool solid;
    bool grounded;

    float mass;
    float movement_speed;
    float jump_force;
    float dash_force;

    time_t dash_tick;
    time_t dash_duration_ms;
    time_t grounded_tick;

    void (*collision_callback)(struct entity_s *, struct entity_s *);
} rigid_body_t;

typedef struct {
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    bool solid;
    void (*collision_callback)(struct tile_s *, struct entity_s *);
} static_body_t;

#endif

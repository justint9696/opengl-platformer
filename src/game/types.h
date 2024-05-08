#pragma once

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <time.h>

struct entity_s;

typedef enum {
    DIR_UP = 1 << 0,
    DIR_DOWN = 1 << 1,
    DIR_LEFT = 1 << 2,
    DIR_RIGHT = 1 << 3,
} direction_t;

typedef struct {
    vec2s pos, dim;
} Box;

typedef struct {
    union {
        Box box;
        struct {
            vec2s pos, dim;
        };
    };
    vec2s vel;
    bool grounded;
    time_t grounded_tick;
    direction_t direction;
    void (*collision_callback)(struct entity_s *, struct entity_s *, float dt);
} rigid_body_s;

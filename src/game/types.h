#pragma once

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <time.h>

struct entity_s;

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
    bool grounded;
    time_t grounded_tick;
    void (*collision_callback)(struct entity_s *, struct entity_s *);
} RigidBody;

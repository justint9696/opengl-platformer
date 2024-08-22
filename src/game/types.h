#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include "util/aabb.h"

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <time.h>

struct entity_s;
struct cell_s;
struct page_s;

typedef struct {
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };
    void *data;
    void (*callback)(void *, void *);
} collider_t;

typedef struct {
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    struct page_s *page;
    struct cell_s *cell;

    vec2s vel;
    vec2s force;

    bool solid;
    bool grounded;

    float mass;
    float speed;
    float jump_force;
    float dash_force;

    time_t dash_tick;
    time_t jump_tick;

    void *collision_callback;
} body_t;

#endif // ifndef _GAME_TYPES_H_

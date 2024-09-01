/**
 * @file types.h
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Common game type data structures.
 */

#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include "util/aabb.h"

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <time.h>

struct entity_s;
struct cell_s;
struct page_s;

/** @brief Data structure for collision callbacks. */
typedef struct collider_s {
    /** @brief The physical bounds of the collider. */
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    /** @brief A pointer to the data of the collider. */
    void *data;

    /** @brief Optional collision callback function pointer. */
    void (*callback)(void *, void *);
} collider_t;

/** @brief Represents a physical game body. */
typedef struct body_s {
    /** @brief The physical bounds of the body. */
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    /** @brief The current page the body is in. */
    struct page_s *page;

    /** @brief The current cell the body is in. */
    struct cell_s *cell;

    /** @brief The current speed of the body. */
    vec2s vel;

    /** @brief The current forces acting on the body. */
    vec2s force;

    /** @brief Enables collision with other bodies. */
    bool solid;

    /** @brief The current grounded state of the body. */
    bool grounded;

    /** @brief The mass of the body. */
    float mass;

    /** @brief The terminal speed of the body. */
    float speed;

    /** @brief The force applied when jumping. */
    float jump_force;

    /** @brief The force applied when dashing. */
    float dash_force;

    /** @brief The game tick the dash was started. */
    time_t dash_tick;
    
    /** @brief The game tick the jump was started. */
    time_t jump_tick;

    /** @brief Optional collision callback function pointer. */
    void *collision_callback;
} body_t;

#endif // ifndef _GAME_TYPES_H_

#ifndef _UTIL_AABB_H_
#define _UTIL_AABB_H_

#include <cglm/types-struct.h>
#include <stdbool.h>

typedef struct {
    vec2s pos, dim;
} aabb_t, box_t;

/* Returns the center point of an aabb. */
vec2s box_center(const box_t *);

/* Creates a broad-phase bounding box (bbb) along the provided axis or all axes
 * if -1. */
void bbb_create(box_t *, vec2s vel, short axis, float dt);

/* Returns true if two aabb's are overlapping or false otherwise. */
bool aabb_collision_2d(const box_t *, const box_t *);

#endif // ifndef _UTIL_AABB_H_

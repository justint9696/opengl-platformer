#ifndef _UTIL_AABB_H_
#define _UTIL_AABB_H_

#include <cglm/types-struct.h>
#include <stdbool.h>

typedef struct {
    vec2s pos, dim;
} aabb_t, box_t;

/* Returns the center point of an aabb. */
vec2s box_center(const box_t *);

/* Creates a broad-phase bounding box (bbb) along the provided axis. */
void bbb_create(box_t *, vec2s vel, int axis, float dt);

/* Returns true if two aabb's are overlapping or false otherwise. */
bool aabb_collision_2d(const box_t *, const box_t *);

/* Returns true if a position overlaps an aabb or false otherwise. */
bool aabb_intersect_2d(const box_t *, vec2s pos);

#endif // ifndef _UTIL_AABB_H_

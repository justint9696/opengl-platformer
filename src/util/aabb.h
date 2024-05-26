#ifndef _UTIL_AABB_H_
#define _UTIL_AABB_H_

#include <cglm/types-struct.h>
#include <stdbool.h>

typedef struct {
    vec2s pos, dim;
} aabb_t, box_t;

vec2s box_center(const box_t *);
void bbb_create(box_t *, vec2s vel, short axis);
bool aabb_collision_2d(const box_t *, const box_t *);

#endif

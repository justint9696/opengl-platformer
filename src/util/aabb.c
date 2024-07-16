#include "util/aabb.h"

#include <cglm/struct.h>

vec2s box_center(const box_t *self) {
    return glms_vec2_muladds(self->dim, 0.5f, self->pos);
}

void bbb_create(box_t *self, vec2s vel, short axis, float dt) {
    vel = glms_vec2_scale(vel, dt);
    self->dim.raw[axis] += fabsf(vel.raw[axis]);
    self->pos.raw[axis] = (vel.raw[axis] >= 0.f)
                              ? self->pos.raw[axis]
                              : self->pos.raw[axis] + vel.raw[axis];
}

inline bool aabb_collision_2d(const box_t *a, const box_t *b) {
    return ((a->pos.x < b->pos.x + b->dim.x) 
            && (a->pos.x + a->dim.x > b->pos.x)
            && (a->pos.y < b->pos.y + b->dim.y)
            && (a->pos.y + a->dim.y > b->pos.y));
}

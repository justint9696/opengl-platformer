#include "entity/entity.h"
#include "game/defs.h"
#include "util/util.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>

static const vec2s gravity = (vec2s) { .x = 0.f, .y = -GRAVITY };

void physics_tick(entity_t *self, world_t *world, float dt) {
    if (self->flags & F_GRAVITY) {
        self->body.vel = glms_vec2_add(self->body.vel, gravity);
        /* self->body.vel = glms_vec2_muladds(gravity, dt, self->body.vel); */
    }
    self->body.vel.raw[0]
        = clamp(self->body.vel.raw[0], -self->body.movement_speed,
                self->body.movement_speed);
    self->body.vel.raw[1] = max(self->body.vel.raw[1], gravity.y * 2.f);
}

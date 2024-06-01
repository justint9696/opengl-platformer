#include "entity/entity.h"
#include "game/defs.h"
#include "util/util.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>

static const vec2s gravity
    = (vec2s) { .x = 0.f, .y = -GRAVITY * MOVEMENT_SCALAR };

void physics_tick(entity_t *self, world_t *world, float dt) {
    // mid-air drag
    vec2s movement = glms_vec2_normalize(self->body.movement);
    if (movement.x == 0.f) {
        self->body.vel.x = 0.f;
    } else if (self->flags & F_GRAVITY) {
        self->body.vel.x
            += (movement.x
                * (self->body.movement_speed
                   * (self->body.grounded ? 1.f : DRAG_COEFFICIENT)))
               * MOVEMENT_SCALAR;
    }

    // player jump
    if (movement.y > 0.f) {
        self->body.vel.y = self->body.jump_speed * MOVEMENT_SCALAR;
        self->body.grounded = false;
    }

    // gravity
    if (self->flags & F_GRAVITY) {
        self->body.vel = glms_vec2_muladds(
            glms_vec2_scale(gravity, self->body.mass), dt, self->body.vel);
    }

    // clamp velocity at terminal values
    self->body.vel.x
        = clamp(self->body.vel.x, -self->body.movement_speed * MOVEMENT_SCALAR,
                self->body.movement_speed * MOVEMENT_SCALAR);
    self->body.vel.y = max(self->body.vel.y, gravity.y);
}

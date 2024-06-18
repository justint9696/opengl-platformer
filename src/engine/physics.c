#include "entity/entity.h"
#include "game/constants.h"
#include "util/time.h"
#include "util/util.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>

static const vec2s gravity
    = (vec2s) { .x = 0.f, .y = -GRAVITY * MOVEMENT_SCALAR };

static const vec2s drag = (vec2s) { .x = DRAG_COEFFICIENT, .y = 0.f };

void physics_sync(entity_t *self, world_t *world, float dt) {
    vec2s movement = glms_vec2_normalize(self->body.vdir);

    if (movement.x == 0.f) {
        self->body.vel.x = 0.f;
    } else if (self->flags & F_GRAVITY) {
        // apply drag if entity is not grounded
        float vel_x = movement.x * self->body.movement_speed;
        vel_x *= (self->body.grounded) ? 1.f : drag.x;

        self->body.vel.x += (vel_x * MOVEMENT_SCALAR);
    }

    // entity dash
    if (time_since_ms(self->body.dash_tick) < 250) {
        self->body.accel.x
            += self->body.vdir.x * self->body.dash_force * MOVEMENT_SCALAR;
    }

    // entity jump
    if (movement.y > 0.f) {
        self->body.vel.y = self->body.jump_force * MOVEMENT_SCALAR;
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

    // apply forces to velocity vector
    self->body.vel = glms_vec2_add(self->body.accel, self->body.vel);
}

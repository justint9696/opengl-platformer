#include "entity/entity.h"
#include "game/constants.h"
#include "game/input.h"
#include "util/time.h"
#include "util/util.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>

static const vec2s gravity
    = (vec2s) { .x = 0.f, .y = -GRAVITY * MOVEMENT_SCALAR };

static const vec2s drag = (vec2s) { .x = DRAG_COEFFICIENT, .y = 0.f };

void physics_sync(entity_t *self, world_t *world, float dt) {
    vec2s movement = GLMS_VEC2_ZERO_INIT;
    if (self->buttons & BUTTON_LEFT) {
        movement.x -= 1.f;
    }

    if (self->buttons & BUTTON_RIGHT) {
        movement.x += 1.f;
    }

    if (self->buttons & BUTTON_SPACE) {
        movement.y += 1.f;
    }

    if (self->buttons & BUTTON_RSHIFT) {
        self->body.dash_tick = NOW();
    }

    if (movement.x == 0.f) {
        self->body.vel.x = 0.f;
    } else if (self->flags & F_GRAVITY) {
        // apply drag if entity is not grounded
        float xvel = movement.x * self->body.speed;
        xvel *= (self->body.grounded) ? 1.f : drag.x;

        self->body.vel.x += (xvel * MOVEMENT_SCALAR);
    }

    // entity dash
    if (time_since_ms(self->body.dash_tick) < 250) {
        self->body.force.x
            += movement.x * self->body.dash_force * MOVEMENT_SCALAR;
    }

    // entity jump
    if (movement.y > 0.f) {
        self->body.vel.y = self->body.jump_force * MOVEMENT_SCALAR;
    }

    // apply gravity
    if (self->flags & F_GRAVITY) {
        self->body.vel = glms_vec2_muladds(
            glms_vec2_scale(gravity, self->body.mass), dt, self->body.vel);
    }

    // clamp velocity at terminal values
    self->body.vel.x
        = clamp(self->body.vel.x, -self->body.speed * MOVEMENT_SCALAR,
                self->body.speed * MOVEMENT_SCALAR);
    self->body.vel.y = max(self->body.vel.y, gravity.y);

    // apply forces to velocity vector
    self->body.vel = glms_vec2_add(self->body.force, self->body.vel);
}

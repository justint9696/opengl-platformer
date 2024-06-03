#include "entity/entity.h"
#include "world/world.h"

#include <cglm/struct.h>

void movement_tick(entity_t *self, world_t *world, float dt) {
    // store current position
    vec2s prev_pos = world_to_screen(world, self->body.pos);

    // apply movement
    self->body.pos = glms_vec2_muladds(self->body.vel, dt, self->body.pos);
    vec2s pos = world_to_screen(world, self->body.pos);

    // update position within uniform grid
    if (self->body.solid) {
        grid_update(&world->grid, &self, prev_pos, pos);
    }

    // reset movement and acceleration vectors
    self->body.direction = GLMS_VEC2_ZERO;
    self->body.accel = GLMS_VEC2_ZERO;
}

#include "data/array.h"
#include "entity/entity.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>
#include <stdbool.h>

static void resolve_collision(entity_t *self, world_t *world, box_t *box,
                              vec2s *movement, int axis) {
    // set position to entry point of collision along the current axis
    if (movement->raw[axis] < 0.000001f) {
        self->body.pos.raw[axis] = box->pos.raw[axis] + box->dim.raw[axis];
    } else if (movement->raw[axis] > 0.000001f) {
        self->body.pos.raw[axis]
            = box->pos.raw[axis] - self->body.dim.raw[axis];
    }

    // stop further movement along collision axis
    movement->raw[axis] = 0.f;
}

static vec2s try_move(entity_t *self, world_t *world, collider_t arr[],
                      size_t len, float dt) {
    vec2s movement = glms_vec2_make(self->body.vel.raw);

    // check for collision with entities
    for (size_t i = 0; i < len; i++) {
        collider_t *tmp = &arr[i];
        for (int i = 0; i < 2; i++) {
            if (fabsf(movement.raw[i]) <= 0.000001f)
                continue;

            box_t bbb = self->body.box;
            bbb_create(&bbb, movement, i, dt);
            if (aabb_collision_2d(&bbb, &tmp->box)) {
                resolve_collision(self, world, &tmp->box, &movement, i);
                if (tmp->callback) {
                    tmp->callback(tmp->data, self);
                }
            }
        }
    }

    return movement;
}

void collision_sync(entity_t *self, world_t *world, float dt) {
    // get possible colliders from uniform grid
    collider_t arr[64];
    size_t n = world_get_colliders(world, self, arr, 64);

    // attempt to move entity
    vec2s movement = try_move(self, world, arr, n, dt);

    // entity is grounded if downward velocity was stopped
    self->body.grounded = self->body.vel.y < 0 && movement.y == 0;

    self->body.vel = glms_vec2_make(movement.raw);
}

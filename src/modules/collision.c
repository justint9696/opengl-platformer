#include "data/array.h"
#include "data/kdtree.h"
#include "entity/entity.h"
#include "game/types.h"
#include "util/aabb.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>
#include <stdbool.h>

static void handle_collision(box_t *a, box_t *b, vec2s *movement, short axis) {
    // set position to entry point of collision along the current axis
    if (movement->raw[axis] < 0.f) {
        a->pos.raw[axis] = b->pos.raw[axis] + b->dim.raw[axis];
    } else if (movement->raw[axis] > 0.f) {
        a->pos.raw[axis] = b->pos.raw[axis] - a->dim.raw[axis];
    }

    // stop further movement along collision axis
    movement->raw[axis] = 0.f;
}

static vec2s try_move(entity_t *self, world_t *world, void *arr[], size_t len,
                      float dt) {
    vec2s movement = glms_vec2_make(self->body.vel.raw);

    // check for collision with entities
    for (size_t i = 0; i < len; i++) {
        bool collision = false;
        entity_t *tmp = arr[i];
        for (short i = 0; i < 2; i++) {
            if (movement.raw[i] == 0.f)
                continue;

            box_t bbb = self->body.box;
            bbb_create(&bbb, movement, i, dt);
            if (aabb_collision_2d(&bbb, &tmp->body.box)) {
                // handle collision along axis
                handle_collision(&self->body.box, &tmp->body.box, &movement, i);
                collision = true;
            }
        }
        if (collision && tmp->body.collision_callback) {
            tmp->body.collision_callback(tmp, self);
        }
    }

    // check for collision with tiles
    for (short i = 0; i < 2; i++) {
        if (movement.raw[i] == 0.f)
            continue;

        box_t bbb = self->body.box;
        bbb_create(&bbb, movement, i, dt);
        tile_t *tmp = kdtree_nearest(&world->kdtree, box_center(&bbb));
        assert(tmp);

        if (aabb_collision_2d(&bbb, &tmp->body.box)) {
            /* printf("%d\n", tmp->id); */
            // handle collision along axis
            handle_collision(&self->body.box, &tmp->body.box, &movement, i);
            if (tmp->body.collision_callback) {
                tmp->body.collision_callback(tmp, self);
            }
        }
    }

    return movement;
}

void collision_tick(entity_t *self, world_t *world, float dt) {
    // get possible colliders from uniform grid
    entity_t *arr[64];
    size_t n = world_get_colliders(world, self, arr, 64);

    // attempt to move entity
    vec2s movement = try_move(self, world, (void **)arr, n, dt);

    // entity is grounded if downward velocity was stopped
    self->body.grounded = self->body.vel.y < 0 && movement.y == 0;

    self->body.vel = glms_vec2_make(movement.raw);
}
